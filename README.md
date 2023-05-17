# Introduction

Final proyects for the course on Advanced Algorithms. Each proyect solves a different problem, outlined as follows. 

# Proyect 1

**Part 1:**
Objective: Search for malicious codes in transmission files
Algorithm: Uses the Z algorithm
Complexity: O(N)

The Z algorithm is used to determine if malicious content is present in the given transmission files. The algorithm works as follows: Given a string and a pattern (in this case, the transmission and the malicious code, respectively), an integer array of size string.size() is created to represent the number of characters that match the given pattern. If the length of the largest prefix of the pattern is found in the string, it means that the pattern is present.

**Part 2:**
Objective: Search for the longest palindrome
Algorithm: Manacher's algorithm
Complexity: O(N)

Manacher's algorithm is used to find the largest palindrome within a text string. In this case, the text strings are the transmission files, and the goal is to find the longest palindrome assuming that the malicious code is a palindrome. Manacher's algorithm takes advantage of the symmetry of palindromes to avoid redundant computations by recognizing that there are palindromes within palindromes. The algorithm has a linear complexity. Here's how it works: A list of the same length as the text string is created to store the lengths of the palindromes. The algorithm iterates character by character, taking each character as the center of a palindrome. Assuming the standard case where there is symmetry and the sides are equal, and not reaching the limits of the array, the algorithm calculates the length of the palindrome and stores it in the list. The algorithm also considers special cases where the palindrome extends beyond the current boundaries. As lengths are stored in the list, it checks if the current length is the longest encountered so far and updates the variable that holds the longest length accordingly. This makes it easy to display the starting and ending positions of the longest palindrome after the iteration is complete.

**Part 3:**
Case where the left sub-palindrome extends beyond the longest palindrome and cannot be matched by the right sub-palindrome.
Case where both left and right palindromes have the same conditions, but the right one still has room to grow.
Objective: Search for the longest common substring
Complexity: O(N*M)
Algorithm: Dynamic Programming

Dynamic Programming is used to find the longest common substring between two different text strings. In this case, the two text strings are the transmission files. The algorithm starts by creating a table of size text string 1 length by text string 2 length, which stores the lengths of the common suffixes. Both text strings are iterated using two nested loops, allowing the lengths of the common suffixes to be stored at the appropriate indices. This iteration also enables the algorithm to keep track of the longest common substring length and its location. Once the iteration is complete, it is straightforward to display the results by having the initial index of the substring and its length (the length of the longest common substring).

# Description: Proyect 2

Given an input file containing X, Y coordinates of neighborhoods and substations, as well as a flow matrix between the neighborhoods. Utilize the neighborhood coordinates to generate an adjacency matrix representing a weighted undirected graph, where the distances between neighborhoods are calculated using the Euclidean formula.

Task 1: Print the adjacency matrix with the distances between the neighborhoods.
Task 2: Print the most optimal way to connect the neighborhoods.
Task 3: Print the shortest path with the minimum cost to visit all neighborhoods starting from an origin neighborhood and returning to it.
Task 4: Print the maximum flow between each neighborhood i and neighborhood j using the flow matrix between neighborhoods.
Task 5: Print the nearest substation and neighborhood geographically to a new hire.

# Solution: Proyect 2

For task 1, which is to print the adjacency matrix with distances between the neighborhoods, we initially fill the matrix using a function that calculates the distance between two neighborhoods using their coordinates with the Euclidean equation: sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)). Then, to print this matrix, we use two for loops, resulting in a complexity of O(N^2).

For task 2, we print the most optimal way to connect the neighborhoods. We achieve this using the Prim's algorithm, a greedy algorithm that utilizes a minimum spanning tree to display the lowest-cost way of connecting the neighborhoods. To print the minimum spanning tree, we implemented the following algorithm: After creating the minimum spanning tree, we maintain two vectors representing the neighborhoods already included in the MST and the neighborhoods not yet included. We iterate through the adjacency matrix, considering all connections between neighborhoods and selecting the minimum distance. This neighborhood with the lowest cost is included in the tree. Finally, we display the list of edges representing pairs for each neighborhood and their respective lengths, showing the most optimal way to connect them. The complexity of this algorithm ended up being O(V^2).

For task 3, we were asked to print the shortest path with the minimum cost to visit all neighborhoods starting from an origin neighborhood and returning to it. This problem is based on the Traveling Salesman Problem. The solution implemented for this problem is quite interesting, as it uses an auxiliary recursive function and achieves a complexity of O(N^2). The algorithm works as follows: first, we generate the path with the minimum spanning tree using the recursive auxiliary function inOrder, filtering distances and keeping the smallest ones. Then, we iterate through the matrix, and for each neighborhood, we create a vector to represent the current path, another vector to represent the visited nodes, and a variable to represent the current cost. We then iterate in a for loop, skipping repeated nodes in the walk (vector calculated with the recursive auxiliary function), and update the current path accordingly. After that, we close the cycle by returning to the starting point. Then, we add up the distance of the chosen neighborhoods within the currentPath (iterating currentPath) and store the cost in the current cost variable. Finally, we update the minimum cost variable if the current cost is lower. Similarly, if this condition is met, the bestPath vector is updated with the values from currentPath. After iterating through the matrix, we finally print the results.

For task 4, we print the maximum flow between each neighborhood i and neighborhood j using the flow matrix between neighborhoods. We achieve this by implementing the Ford-Fulkerson algorithm. It works as follows: we first initialize the working variables, including a vector of vectors to store the residual values, two vectors, and a vector to store the ancestor nodes. The vector of vectors of residual nodes is initialized with the flow matrix. The ancestor nodes vector is initialized with the size of the matrix and all values set to 0. We then implement a while loop that runs as long as a path exists. This is determined using an auxiliary function that utilizes a stack and a visited vector, iterating through the residuals and returning true if a path from the initial node to the final node exists. While this condition is met, for each node X from the final node, until X is not equal to the initial node, and with each iteration changing X to the value of ancestors at index X, we update the flow value with the minimum of the current value and the residual value at [U][X], where U is the value of ancestors at index X. Then, we iterate again with the same conditions as the previous for loop, but now we update the residual values. Finally, we add the flow value to

the maximum flow variable. After the loop finishes, we display the maximum flow. It's worth noting that the original Ford-Fulkerson algorithm has a complexity that depends on the number of paths that make up the maximum flow F, O(EF). However, the use of a matrix and the BFS auxiliary function results in a complexity of O(N^5).

Finally, for task 5, we were asked to print the nearest substation and neighborhood geographically to a new connection. We were able to solve this problem by implementing concepts of computational geometry. We use two nested loops, where one checks the nearest neighborhood and the other checks the nearest substation to the new connection. This is done for each of the new connections. In the loop to check the nearest substation to the new connection, we use an auxiliary function that calculates the distance between this new connection and the available substations using the Euclidean equation. We store the minimum distance in a variable and update it when the calculated distance is smaller than the current minimum; if this condition is met, we also update the nearest substation. Similarly, a similar approach is used to find the nearest neighborhood to the new connection. In the loop to check the nearest neighborhood to the new connection, we use an auxiliary function that calculates the distance between this new connection and the available neighborhoods using the Euclidean equation. We store the minimum distance in a variable and update it when the calculated distance is smaller than the current minimum; if this condition is met, we also update the nearest neighborhood. Finally, for each new connection, we display the nearest neighborhood and its respective distance, as well as the nearest substation and its respective distance. The complexity of this algorithm is O(N^2) due to the two nested loops.



