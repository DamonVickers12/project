/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <ctime>

#define SIZE 20

using namespace std;

// Function to find the minimum in a given array segment
int find_minimum(int arr[], int start, int end) {
    int min_val = arr[start];
    for (int i = start + 1; i < end; i++) {
        if (arr[i] < min_val) {
            min_val = arr[i];
        }
    }
    return min_val;
}

int main() {
    srand(time(0));
    int arr[SIZE];
    int pipefd[2]; // Pipe for inter-process communication
    pid_t pid;
    
    // Initialize array with random numbers
    cout << "Generated array: ";
    for (int i = 0; i < SIZE; i++) {
        arr[i] = rand() % 100; // Random numbers between 0 and 99
        cout << arr[i] << " ";
    }
    cout << endl;
    
    // Create a pipe
    if (pipe(pipefd) == -1) {
        cerr << "Pipe creation failed" << endl;
        return 1;
    }

    // Fork a child process
    pid = fork();
    
    if (pid < 0) {
        cerr << "Fork failed" << endl;
        return 1;
    } 
    else if (pid == 0) { // Child process
        close(pipefd[0]); // Close unused read end
        int min_child = find_minimum(arr, SIZE / 2, SIZE);
        write(pipefd[1], &min_child, sizeof(min_child));
        close(pipefd[1]); // Close write end after sending data
        cout << "Child Process (PID: " << getpid() << ") found min: " << min_child << endl;
        exit(0);
    } 
    else { // Parent process
        close(pipefd[1]); // Close unused write end
        int min_parent = find_minimum(arr, 0, SIZE / 2);
        int min_child;
        read(pipefd[0], &min_child, sizeof(min_child));
        close(pipefd[0]); // Close read end after receiving data
        
        cout << "Parent Process (PID: " << getpid() << ") found min: " << min_parent << endl;
        wait(NULL); // Wait for child process to complete
        
        // Find overall minimum
        int overall_min = (min_parent < min_child) ? min_parent : min_child;
        cout << "Overall minimum number in the array: " << overall_min << endl;
    }
    
    return 0;
}
