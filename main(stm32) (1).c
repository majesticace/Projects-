/*
 * main.c
 *
 *  Created on: Nov 11, 2023
 *      Author: Sharon Peters
 *      Purpose: a project that will allow any random set of grades between
 *       0 and 100 inclusive to be analyzed
 */
#include <stdio.h>

// Declare an array of grades
int grades[] = {77, 44, 22, 88, 11, 33, 45, 67, 32, 89, 27};

// Define a struct to store the answers
struct Answers
{
    int min_Value;
    int max_Value;
    int median;
    int mean;
    int As;
    int Bs;
    int Cs;
    int Ds;
    int Fs;
} ans;

// Full Functions (functions to find answ)
int findMinGrade(int grades[], int numGrades);
int findMaxGrade(int grades[], int numGrades);
int calculateMedian(int grades[], int numGrades);
int calculateMean(int grades[], int numGrades);
void countLetterGrades(int grades[], int numGrades, struct Answers *ans);

int main(void)
{
    // Calculate the number of grades in the array
    int numGrades = sizeof(grades) / sizeof(grades[0]);

    // Call the functions to compute the answers
    ans.min_Value = findMinGrade(grades, numGrades);
    ans.max_Value = findMaxGrade(grades, numGrades);
    ans.median = calculateMedian(grades, numGrades);
    ans.mean = calculateMean(grades, numGrades);
    countLetterGrades(grades, numGrades, &ans);

    // Infinite loop to keep the program running
    while (1);
}

// Function to find the minimum grade
int findMinGrade(int grades[], int numGrades)
{
    // Assume the first grade is the minimum
    int minGrade = grades[0];

    // Iterate through the remaining grades
    for (int i = 1; i < numGrades; i++)
    {
        // Update the minimum if a lower grade is found
        if (grades[i] < minGrade)
        {
            minGrade = grades[i];
        }
    }

    // Return the minimum grade
    return minGrade;
}

// Function to find the maximum grade
int findMaxGrade(int grades[], int numGrades)
{
    // Assume the first grade is the maximum
    int maxGrade = grades[0];

    // Iterate through the remaining grades
    for (int i = 1; i < numGrades; i++)
    {
        // Update the maximum if a higher grade is found
        if (grades[i] > maxGrade)
        {
            maxGrade = grades[i];
        }
    }

    // Return the maximum grade
    return maxGrade;
}

// Function to calculate the median grade
int calculateMedian(int grades[], int numGrades)
{
    // Sort the grades array in ascending order
    for (int i = 0; i < numGrades - 1; i++)
    {
        for (int j = 0; j < numGrades - i - 1; j++)
        {
            // Swap grades if they are out of order
            if (grades[j] > grades[j + 1])
            {
                int temp = grades[j];
                grades[j] = grades[j + 1];
                grades[j + 1] = temp;
            }
        }
    }

    // Calculate the median
    if (numGrades % 2 == 0)
    {
        // If the number of grades is even, average the two middle grades
        return (grades[numGrades / 2 - 1] + grades[numGrades / 2]) / 2;
    }
    else
    {
        // If the number of grades is odd, return the middle grade
        return grades[numGrades / 2];
    }
}

// Function to calculate the mean grade
int calculateMean(int grades[], int numGrades)
{
    // Compute the sum of all grades
    int sum = 0;
    for (int i = 0; i < numGrades; i++)
    {
        sum += grades[i];
    }

    // Calculate the mean by dividing the sum by the number of grades
    return sum / numGrades;
}

// Function to count letter grades
void countLetterGrades(int grades[], int numGrades, struct Answers *ans)
{
    // Initialize grade counters to zero
    ans->As = 0;
    ans->Bs = 0;
    ans->Cs = 0;
    ans->Ds = 0;
    ans->Fs = 0;

    // Iterate through all grades
    for (int i = 0; i < numGrades; i++)
    {
        // Increment the corresponding grade counter based on the grade range
        if (grades[i] >= 90)
        {
            ans->As++;
        }
        else if (grades[i] >= 80)
        {
            ans->Bs++;
        }
        else if (grades[i] >= 70)
        {
            ans->Cs++;
        }
        else if (grades[i] >= 60)
        {
            ans->Ds++;
        }
        else
        {
            ans->Fs++;
        }
    }
}


