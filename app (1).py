"""app.py"""

import boto3
import datetime
import time
import json
import streamlit as st
import pandas as pd
import numpy as np
import plotly.express as px
import plotly.graph_objects as go

AWS_S3_BUCKET = "MY_BUCKET" #edit this
AWS_S3_KEY_PREFIX = "MY_KEY_PREFIX" #edit this NOTE: refers to s3 object key
													 #not access key
s3 = boto3.resource("s3")
bucket = s3.Bucket(AWS_S3_BUCKET)

#time in seconds between updates
interval = 10

def get_data_aws(s3bucket):
	#get data from S3 bucket and return in dictionary format
	for obj in s3bucket.objects.all():
		key = obj.key
		body = obj.get()['Body'].read().decode('utf-8')
		json_content = json.loads(body)
		return json_content

#Get an initial reading
data = get_data_aws(bucket)
df = pd.DataFrame([data])

#Set title of webpage and set up 2 empty containers
st.set_page_config(layout="wide")
st.title('CPE 4040 Weather Station')
placeholder = st.empty()

while True:
	data = get_data_aws(bucket)
	new_df = pd.DataFrame([data])
	df = pd.concat([df, new_df], ignore_index=True)
	df = df.tail(24)

	current_temp = df['temp'].tail(1)
	current_humidity = df['humidity'].tail(1)

	with placeholder.container():
		#3 columns for metrics under title
		metric1, metric2, metric3 = st.columns(3)

		metric1.metric(
    			label = 'Current Temp (C)',
    			value = current_temp)

		metric2.metric(
    			label = 'Current Humidity (%)',
    			value = current_humidity)

		metric3.metric(
			label = 'Average Temp (C)',
			value = round(df['temp'].mean(),2))

		#2 columns for line charts
		fig_col1, fig_col2 = st.columns(2)

		with fig_col1:
			#generate a line-chart of the temp values
			temp_chart = px.line(df,x='timestamps',y='temp',title='temp')
			#get rid of ticks on x-axis because unix timestamps aren't readable
			temp_chart.update_xaxes(showticklabels=False)
			#explicitly set y-axis scaling and increase font-size
			temp_chart.update_yaxes(range=[0, 100])
			st.write(temp_chart)

		with fig_col2:
			humidity_chart = px.line(df,x='timestamps',y='humidity',title='humidity')
			humidity_chart.update_xaxes(showticklabels=False)
			humidity_chart.update_yaxes(range=[0, 100])
			st.write(humidity_chart)

		gauge_chart = go.Figure(go.Indicator(
			mode   = 'gauge+number+delta',
			value  = int(current_humidity),
			domain = {'x':[0,1],'y':[0,1]},
			delta  = {'reference':50},
			title  = {'text':'Humidity'},
			gauge  = {'axis':   {'range':[0,100]},
				  'steps': [{'range': [0,50],  'color':'yellow'},
					    {'range': [50,75], 'color':'orange'},
					    {'range': [75,100],'color':'red'   }],}))
		st.write(gauge_chart)

	time.sleep(interval)
