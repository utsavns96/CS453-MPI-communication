# CS453-MPI-communication
Repository for Homework 1 of CS453 @ UIC Fall 2022

## **CS 453 - Introduction to Parallel and Distributed Processing**

# **Parallel Processing**

## **Formulations**

### **Program 1:**

The first program in our homework adds the array by using the MPI_SEND() and MPI_RECV() functions. In this program, we input the number of processors that we have running and the size of the array that we want to split and add as command line arguments. To start, we initialize our variables, and then initialize out MPI environment. We define the variable to decide how many elements the sub-array should contain by dividing the size of the array that we receive in the arguments and divide it by the number of processors that we have.

Now, to create the array, we check if we are running at processor 0 – the root node in our environment, and then fill in the values for our array A in this node.

We use the MPI_Barrier() function to let all our processors reach the same point and complete their initialization before we proceed. Now, if the processor rank is 0, we check if we have more than 1 processor running, in which case we create a for loop to send our array. This array iterates from the first processor in our world, to worldsize-1, since one of our nodes is the root that we are sending from. In this loop, we perform our MPI_Send() command. The value is defined as the element of the array that is at position 'processor number \* number of elements per sub-array', and we send the number of elements per-subarray that we calculated before. On the other nodes, we receive these elements using MPI_Recv().

Now to calculate our sum, we first calculate the sum of our subarrays at all nodes, including the root processor 0. Then, we use MPI_Send() from our child nodes and use MPI_Recv() at our root node to collect our subarray sums. Then, we add up these sums to the subarray sum we calculated for our root node, and this gives us the total sum which we print from processor 0.

### **Program 2:**

In this program, we add the elements in our array using the MPI_Scatter and MPI_Gather functions. We take the number of running processes and the size of the array that we want to split and add, as input from the command line arguments. We initialize the variables and then initialize the MPI Environment using MPI_Init().

We then check if the program is running in processor 0 (the root node) and use the rand_num_gen() to initialize the array with a set of random numbers. After generation of the random array, we allocate space of the subarrays that will be passed on, to the sub-processes for addition.

We then use the MPI_Scatter() function to send the data from processor 0 (root) to all other processes in the communicator. The sub-processes then call the addition_func() to add all the part arrays and generate their respective sums of the elements. We then call the MPI_Gather() command to gather all the sums from various processes into the root process. The root node then adds the partial sums returned from all other processes to give the final sum of the array in process 0. We then clear all the pointers using free() and call the MPI_Finalize() to terminate all the processes and execution environment.

### **Program 3:**

In this program we add the numbers in an array using MPI_Scatter() and MPI_Reduce(). We take the number of processors that we would run and the size of the array that we want to split and add as command line arguments. We take the number of processes and the rank of the processes from the MPI package.

If the process is process 0 (the root node) we initialize the array with elements. After which, we call the MPI_Scatter() function to send parts of the arrays in order, to all the processes. We also indicate the receiving parametes and the root process scattering the array and communicator in which the processes reside while making the MPI_Scatter() call. We then call the MPI_Reduce() function which takes an array of elements as input and returns an array of output elements to the root process. The output elements contain the reduced result. We pass the localsum variable to the Reduce function to pass the sum of the numbers added locally. The MPI_SUM carries out a reduction operation on the data and sums all the elements. We implement the MPI_Barrier which blocks the caller until all processes in the communicator have reached the routine.

All processes then call the MPI_Finalize to terminate the MPI execution environment and exit the functioning. The root node (process 0) then displays the sum of the array as output.

### **Program 4:**

This program uses a ring topology to pass around an array of integers along processes. We initialize the variables in the program and take the number of processors that we would run and the size of the array that we want to split and add, as command line arguments. We use a function to decide the size of the subarrays that would be managed by each process. This size of the subarrays is then stored in the size variable. The root node (process 0) then initializes the array with elements. For every other sub-process/ non-root node, we start listening for data by calling the MPI_Recv command.

We check the rank of the sub-process. If the rank is less than the maximum rank, we pass the rest of the partial array to the adjacent process.

The program then calculates the local sum in the sub-process. The local sum is then passed on to the next sub-process using the MPI_Send. The source node does not pass this value to any node. Each sub-node receives the sub-total from its neighboring node using MPI_Recv and adds the cumulative total to itself and passes this sum to the next node. The final total reaches the root node (process 0) and program prints the total of the array. All processes then call the MPI_Finalize to terminate the MPI execution environment and exit the functioning.


### **Program 5:**

This program uses the hypercube algorithm to carry out the addition of the array. It distributes the subarrays in log k logical MPI_Send and MPI_Recv calls. The program takes the number of processors that we have running and the size of the array that we want to add as input through command line arguments. For the process 0 (root node) we first initialize the array with values. We then distribute the array across all sub-processes. For this we run a loop over the dimensions (log n). The root node sends the sub-array out using MPI_Isend while the sub-processes receive these values using the MPI_Recv.

Each sub-process calculates the local sum by adding the elements. We then iterate over the dimension for each sub-process send out their local sum using MPI_Isend to the root node/ process 0. The root node receives these values using MPI_Recv and adds the cumulative to calculate the final total. We then display the total of the array and call MPI_Finalize to terminate the MPI execution environment.


### **Program 6:**

This program creates a cartesian topology of a m-dimensional grid using MPI_Cart_create. The program takes the number of processors that we have running and the size of the array that we want to add as input through command line arguments. We then initialize the MPI Environment and the Cartesian environment. We make use of the determine the process coords in cartesian topology by providing the rank of the group. MPI_Dims_create creates a division of processors in the cartesian grid as per the number of nodes in the grid the number of cartesian dimensions needed. We then define variables to locate if the node is the last one on a particular dimension (using check_last_val) and also identify the number of dimensions with only one process (using single_proc_count).

From process 0, we then initialize the Array with elements. We then get the maximum columns and maximum rows we can have in our cartesian coordinates by calling the MPI_Cart_coords on the last process. For every other sub-process, we allocate a buffer size space for computation and call MPI_Recv to listen for data on the lower dimension. We then send the subarray in the lower level if the node is not at the end of the row with the help of MPI_Cart_shift, which takes in the shift direction and amount and returns the shifted source and destination ranks back. MPI_Isend passes these values to the node with a non-blocking send. A loop over the number of processes propagates the subarrays in all directions using the MPI_Cart_shift and MPI_Isend commands.

After this, each process individually computes the local sum to get a value for its sub-array. All processes listen, to receive the results from other nodes and compute the total for the values that they receive. For all sub-nodes, we use the MPI_Cart_shift and send the result to the next node. Finally, the source node prints all the values that it receives from the sub-nodes from the transmission in the opposite direction and sequence.


## **Test Parameters**

For this homework, we used a number of varying parameters to find the resulting total execution time. For programs 1 through 4, we varied the number of nodes between 4, 8 and 16, with array elements 16, 64, 256, 1024 and 16000.

| **Nodes** | **Array Size** | **Total Time (x 10^-6)** | **Total Time Raw** |
| --- | --- | --- | --- |
| 4 | 16 | | |
| 4 | 64 | | |
| 4 | 256 | | |
| 4 | 1024 | | |
| 4 | 16000 | | |
| 8 | 16 | | |
| 8 | 64 | | |
| 8 | 256 | | |
| 8 | 1024 | | |
| 8 | 16000 | | |
| 16 | 16 | | |
| 16 | 64 | | |
| 16 | 256 | | |
| 16 | 1024 | | |
| 16 | 16000 | | |

For programs 5 and 6, we varied the number of nodes between 8 and 16, with array elements 16, 64, 256, 1024, 16000 and 160000.

| **Nodes** | **Array Size** | **Total Time (x 10^-6)** | **Total Time Raw** |
| --- | --- | --- | --- |
| 8 | 16 | | |
| 8 | 64 | | |
| 8 | 256 | | |
| 8 | 1024 | | |
| 8 | 16000 | | |
| 8 | 160000 | | |
| 16 | 16 | | |
| 16 | 64 | | |
| 16 | 256 | | |
| 16 | 1024 | | |
| 16 | 16000 | | |
| 16 | 160000 | | |

## **Results**

### **Program 1:**
<br>

![](/Images/P1_result.jpg)
<br>

| **Nodes** | **Array Size** | **Total Time (x 10^-6)** | **Total Time Raw** |
| --- | --- | --- | --- |
| 4 | 16 | 56 | 0.000056 |
| 4 | 64 | 67 | 0.000067 |
| 4 | 256 | 139 | 0.000139 |
| 4 | 1024 | 369 | 0.000369 |
| 4 | 16000 | 4792 | 0.004792 |
| 8 | 16 | 1200 | 0.0012 |
| 8 | 64 | 1231 | 0.001231 |
| 8 | 256 | 1336 | 0.001336 |
| 8 | 1024 | 1542 | 0.001542 |
| 8 | 16000 | 2843 | 0.002843 |
| 16 | 16 | 2467 | 0.002467 |
| 16 | 64 | 2628 | 0.002628 |
| 16 | 256 | 2336 | 0.002336 |
| 16 | 1024 | 2509 | 0.002509 |
| 16 | 16000 | 6691 | 0.006691 |

### **Program 2:**
<br>

![](/Images/P2_result.jpg)
<br>

| **Nodes** | **Array Size** | **Total Time (x 10^-6)** | **Total Time Raw** |
| --- | --- | --- | --- |
| 4 | 16 | 135 | 0.000135 |
| 4 | 64 | 111 | 0.000111 |
| 4 | 256 | 214 | 0.000214 |
| 4 | 1024 | 1004 | 0.001004 |
| 4 | 16000 | 4919 | 0.004919 |
| 8 | 16 | 510 | 0.00051 |
| 8 | 64 | 694 | 0.000694 |
| 8 | 256 | 605 | 0.000605 |
| 8 | 1024 | 769 | 0.000769 |
| 8 | 16000 | 3126 | 0.003126 |
| 16 | 16 | 1314 | 0.001314 |
| 16 | 64 | 1584 | 0.001584 |
| 16 | 256 | 1209 | 0.001209 |
| 16 | 1024 | 1378 | 0.001378 |
| 16 | 16000 | 3078 | 0.003078 |

### **Program 3:**
<br>

![](/Images/P3_result.jpg)
<br>

| **Nodes** | **Array Size** | **Total Time (x 10^-6)** | **Total Time Raw** |
| --- | --- | --- | --- |
| 4 | 16 | 86 | 0.000086 |
| 4 | 64 | 58 | 0.000058 |
| 4 | 256 | 62 | 0.000062 |
| 4 | 1024 | 72 | 0.000072 |
| 4 | 16000 | 397 | 0.000397 |
| 8 | 16 | 110 | 0.00011 |
| 8 | 64 | 112 | 0.000112 |
| 8 | 256 | 119 | 0.000119 |
| 8 | 1024 | 147 | 0.000147 |
| 8 | 16000 | 635 | 0.000635 |
| 16 | 16 | 171 | 0.000171 |
| 16 | 64 | 181 | 0.000181 |
| 16 | 256 | 190 | 0.00019 |
| 16 | 1024 | 210 | 0.00021 |
| 16 | 16000 | 534 | 0.000534 |

### **Program 4:**
<br>

![](/Images/P4_result.jpg)
<br>

| **Nodes** | **Array Size** | **Total Time (x 10^-6)** | **Total Time Raw** |
| --- | --- | --- | --- |
| 4 | 16 | 74 | 0.000074 |
| 4 | 64 | 49 | 0.000049 |
| 4 | 256 | 166 | 0.000166 |
| 4 | 1024 | 599 | 0.000599 |
| 4 | 16000 | 724 | 0.000724 |
| 8 | 16 | 798 | 0.000798 |
| 8 | 64 | 866 | 0.000866 |
| 8 | 256 | 913 | 0.000913 |
| 8 | 1024 | 922 | 0.000922 |
| 8 | 16000 | 1275 | 0.001275 |
| 16 | 16 | 1577 | 0.001577 |
| 16 | 64 | 1668 | 0.001668 |
| 16 | 256 | 1702 | 0.001702 |
| 16 | 1024 | 2256 | 0.002256 |
| 16 | 16000 | 2280 | 0.00228 |

### **Program 5:**
<br>

![](/Images/P5_result.jpg)
<br>

| **Nodes** | **Array Size** | **Total Time (x 10^-6)** | **Total Time Raw** |
| --- | --- | --- | --- |
| 8 | 16 | 522 | 0.000522 |
| 8 | 64 | 518 | 0.000518 |
| 8 | 256 | 784 | 0.000784 |
| 8 | 1024 | 733 | 0.000733 |
| 8 | 16000 | 730 | 0.00073 |
| 8 | 160000 | 2583 | 0.002583 |
| 16 | 16 | 1444 | 0.001444 |
| 16 | 64 | 1425 | 0.001425 |
| 16 | 256 | 1363 | 0.001363 |
| 16 | 1024 | 1516 | 0.001516 |
| 16 | 16000 | 1582 | 0.001582 |
| 16 | 160000 | 3647 | 0.003647 |

### **Program 6:**
<br>

![](/Images/P6_result.jpg)
<br>

| **Nodes** | **Array Size** | **Total Time (x 10^-6)** | **Total Time Raw** |
| --- | --- | --- | --- |
| 8 | 16 | 775 | 0.000775 |
| 8 | 64 | 803 | 0.000803 |
| 8 | 256 | 753 | 0.000753 |
| 8 | 1024 | 740 | 0.00074 |
| 8 | 16000 | 1026 | 0.001026 |
| 8 | 160000 | 2887 | 0.002887 |
| 16 | 16 | 776 | 0.000776 |
| 16 | 64 | 740 | 0.00074 |
| 16 | 256 | 757 | 0.000757 |
| 16 | 1024 | 794 | 0.000794 |
| 16 | 16000 | 974 | 0.000974 |
| 16 | 160000 | 4216 | 0.004216 |

## **Analysis**

From the numbers, we see a very interesting picture. When adding up 16 elements, the time taken for execution by increasing the number of nodes doesn't decrease accordingly as one might think naively, but actually ends up increasing our execution time. On 4 nodes, we take a measly 0.000056 seconds to add up our elements. By doubling the nodes to 8, our execution time isn't cut in half. Instead, it is increased by over _21 times_ to 0.0012 seconds. The story is similar when we double our nodes yet again, up to 16. This is in reality a pointless exercise, since with 16 elements and 16 nodes, we send 1 element to each node and aren't actually using the nodes to do any addition. This also reflects in the execution time, which is now doubled over 8 nodes to 0.002467. The story is pretty similar for 64, 256 and 1024 element arrays. Adding nodes to solve our computation doesn't give us any benefit and is actually detrimental.

Things take a turn when we move up to 16000 elements. On 4 nodes, our execution time jumps up in an exponential-esq manner to 0.004792 seconds. Here, for the first time, we see a benefit to adding more nodes, since doing the same computation on 8 nodes gives us an execution time of 0.002843 seconds, which is approximately 1.6 times lower than that on 4 nodes – note that a doubling in nodes still doesn't result in halving our execution time. Unfortunately, our jubilation is short-lived, as increasing the number of nodes to 16 net us yet another performance decrease, giving us an execution time of 0.006691 seconds. While all 4, 8 and 16 node setups saw a jump in execution time at 16000 elements, we see the benefit of having more nodes for the first time, but also the detriment of having too many nodes as well. From this, we can extrapolate that there will be yet another, much greater number of elements where we will see the 16-node setup actually return performance improvements over the 8-node setup.
<br>
![](/Images/Analysis_1.jpg)
<br>
In comparison, Program 2 starts off with a higher base execution time of 0.000135 on 4 nodes for 16 elements, but by the time we increase the number of elements to 16000, we see a real performance improvement over the simple Send/Recv method of addition. At 4 nodes with 16,000 elements, we have an execution time of 0.002919, which is approximately a 40% improvement in performance. This story continues for nodes 8 and 16, where we see performance gains ranging from 25% to almost 69%.

| **Nodes** | **Array Size** | **P1** | **P2** | **% Improvement** |
| --- | --- | --- | --- | --- |
| 4 | 16 | 56 | 135 | -141.071 |
| 4 | 64 | 67 | 111 | -65.6716 |
| 4 | 256 | 139 | 214 | -53.9568 |
| 4 | 1024 | 369 | 1004 | -172.087 |
| 4 | 16000 | 4792 | 2919 | 39.08598 |
| 8 | 16 | 1200 | 510 | 57.5 |
| 8 | 64 | 1231 | 694 | 43.62307 |
| 8 | 256 | 1336 | 605 | 54.71557 |
| 8 | 1024 | 1542 | 769 | 50.1297 |
| 8 | 16000 | 2843 | 2126 | 25.21984 |
| 16 | 16 | 2467 | 1314 | 46.73693 |
| 16 | 64 | 2628 | 1384 | 47.33638 |
| 16 | 256 | 2336 | 1209 | 48.24486 |
| 16 | 1024 | 2509 | 1378 | 45.07772 |
| 16 | 16000 | 6691 | 2078 | 68.94336 |

Program 3 gives us an even greater performance improvement over Program 1. Starting off at 4 nodes for 16 elements, even though we see a 53% drop in performance, as soon as we increase our workload, the improvement changes to 13%, and on higher node counts we see an even higher return. In some cases, we see a more than 90% improvement in our execution time, with our greatest returns coming at higher node counts and array sizes.

| **Nodes** | **Array Size** | **P1** | **P3** | **% Improvement** |
| --- | --- | --- | --- | --- |
| 4 | 16 | 56 | 86 | -53.5714 |
| 4 | 64 | 67 | 58 | 13.43284 |
| 4 | 256 | 139 | 62 | 55.39568 |
| 4 | 1024 | 369 | 72 | 80.4878 |
| 4 | 16000 | 4792 | 397 | 91.71536 |
| 8 | 16 | 1200 | 110 | 90.83333 |
| 8 | 64 | 1231 | 112 | 90.90171 |
| 8 | 256 | 1336 | 119 | 91.09281 |
| 8 | 1024 | 1542 | 147 | 90.46693 |
| 8 | 16000 | 2843 | 635 | 77.66444 |
| 16 | 16 | 2467 | 171 | 93.0685 |
| 16 | 64 | 2628 | 181 | 93.11263 |
| 16 | 256 | 2336 | 190 | 91.86644 |
| 16 | 1024 | 2509 | 210 | 91.63013 |
| 16 | 16000 | 6691 | 534 | 92.01913 |

<br>

![](/Images/Analysis_2.jpg)
<br>
Program 3 gives us improvements compared to Program 2 too. Although they are not as high as compared to Program 1, we still see a maximum improvement of 86% in a few cases, with most of our improvements in the 70%-80% range.

| **Nodes** | **Array Size** | **P2** | **P3** | **% Improvement** |
| --- | --- | --- | --- | --- |
| 4 | 16 | 135 | 86 | 36.2963 |
| 4 | 64 | 111 | 58 | 47.74775 |
| 4 | 256 | 214 | 62 | 71.02804 |
| 4 | 1024 | 1004 | 72 | 92.82869 |
| 4 | 16000 | 2919 | 397 | 86.39945 |
| 8 | 16 | 510 | 110 | 78.43137 |
| 8 | 64 | 694 | 112 | 83.86167 |
| 8 | 256 | 605 | 119 | 80.33058 |
| 8 | 1024 | 769 | 147 | 80.88427 |
| 8 | 16000 | 2126 | 635 | 70.1317 |
| 16 | 16 | 1314 | 171 | 86.9863 |
| 16 | 64 | 1384 | 181 | 86.92197 |
| 16 | 256 | 1209 | 190 | 84.28453 |
| 16 | 1024 | 1378 | 210 | 84.76052 |
| 16 | 16000 | 2078 | 534 | 74.30221 |

Coming to program 4, we see that it performs better on average when compared to Program 1. Our speedup ranges from 36% to a peak of almost 93%, but most of our values lie in the 78%- 86% range.

| **Nodes** | **Array Size** | **P1** | **P4** | **% Improvement** |
| --- | --- | --- | --- | --- |
| 4 | 16 | 56 | 74 | -32.1429 |
| 4 | 64 | 67 | 49 | 26.86567 |
| 4 | 256 | 139 | 166 | -19.4245 |
| 4 | 1024 | 369 | 599 | -62.3306 |
| 4 | 16000 | 4792 | 724 | 84.89149 |
| 8 | 16 | 1200 | 798 | 33.5 |
| 8 | 64 | 1231 | 866 | 29.65069 |
| 8 | 256 | 1336 | 913 | 31.66168 |
| 8 | 1024 | 1542 | 922 | 40.20752 |
| 8 | 16000 | 2843 | 1275 | 55.15301 |
| 16 | 16 | 2467 | 1577 | 36.07621 |
| 16 | 64 | 2628 | 1668 | 36.52968 |
| 16 | 256 | 2336 | 1702 | 27.14041 |
| 16 | 1024 | 2509 | 2256 | 10.0837 |
| 16 | 16000 | 6691 | 2280 | 65.92438 |

<br>

![](/Images/Analysis_3.jpg)
<br>
When we check for the improvements compared to program 2, our results are a mixed bag. While we do see performance improvements when we use only 4 nodes, we almost always take a hit to our execution time for 8 and 16 nodes. The only test case in which we see a speedup apart from the ones for the 4 nodes is when we have 8 processors and 16000 array elements.

| **Nodes** | **Array Size** | **P2** | **P4** | **% Improvement** |
| --- | --- | --- | --- | --- |
| 4 | 16 | 135 | 74 | 45.18519 |
| 4 | 64 | 111 | 49 | 55.85586 |
| 4 | 256 | 214 | 166 | 22.42991 |
| 4 | 1024 | 1004 | 599 | 40.33865 |
| 4 | 16000 | 2919 | 724 | 75.19699 |
| 8 | 16 | 510 | 798 | -56.4706 |
| 8 | 64 | 694 | 866 | -24.7839 |
| 8 | 256 | 605 | 913 | -50.9091 |
| 8 | 1024 | 769 | 922 | -19.896 |
| 8 | 16000 | 2126 | 1275 | 40.02822 |
| 16 | 16 | 1314 | 1577 | -20.0152 |
| 16 | 64 | 1384 | 1668 | -20.5202 |
| 16 | 256 | 1209 | 1702 | -40.7775 |
| 16 | 1024 | 1378 | 2256 | -63.7155 |
| 16 | 16000 | 2078 | 2280 | -9.72089 |

Compared to Program 3, it is decidedly worse in every case that we tested. This can be seen by looking at the improvements that we can see in the tables below, which are almost always in the negative, by a huge margin.

| **Nodes** | **Array Size** | **P3** | **P4** | **% Improvement** |
| --- | --- | --- | --- | --- |
| 4 | 16 | 86 | 74 | 13.95349 |
| 4 | 64 | 58 | 49 | 15.51724 |
| 4 | 256 | 62 | 166 | -167.742 |
| 4 | 1024 | 72 | 599 | -731.944 |
| 4 | 16000 | 397 | 724 | -82.3678 |
| 8 | 16 | 110 | 798 | -625.455 |
| 8 | 64 | 112 | 866 | -673.214 |
| 8 | 256 | 119 | 913 | -667.227 |
| 8 | 1024 | 147 | 922 | -527.211 |
| 8 | 16000 | 635 | 1275 | -100.787 |
| 16 | 16 | 171 | 1577 | -822.222 |
| 16 | 64 | 181 | 1668 | -821.547 |
| 16 | 256 | 190 | 1702 | -795.789 |
| 16 | 1024 | 210 | 2256 | -974.286 |
| 16 | 16000 | 534 | 2280 | -326.966 |

Program 5 and Program 6 use slightly different test cases, owing to their respective topologies, which is why we'll take a look at them separately.
<br>
![](/Images/Analysis_4.jpg)
<br>
Program 5 is slightly better than Program 5 is slightly better than Program 6 when we are working with 8 nodes, giving us modest performance gains over Program 6. However, when we bump up our nodes to 16, Program 6 takes the lead, giving us performance increases of approximately 45% across the board, except for when we have 160,000 array elements.
<br>
![](/Images/Analysis_5.jpg)
<br>

| **Nodes** | **Array Size** | **P5** | **P6** | **% Improvement** |
| --- | --- | --- | --- | --- |
| 8 | 16 | 522 | 775 | -48.4674 |
| 8 | 64 | 518 | 803 | -55.0193 |
| 8 | 256 | 784 | 753 | 3.954082 |
| 8 | 1024 | 733 | 740 | -0.95498 |
| 8 | 16000 | 730 | 1026 | -40.5479 |
| 8 | 160000 | 2583 | 2887 | -11.7693 |
| 16 | 16 | 1444 | 776 | 46.26039 |
| 16 | 64 | 1425 | 740 | 48.07018 |
| 16 | 256 | 1363 | 757 | 44.46075 |
| 16 | 1024 | 1516 | 794 | 47.62533 |
| 16 | 16000 | 1582 | 974 | 38.43236 |
| 16 | 160000 | 3647 | 4216 | -15.6019 |

Comparing all 6 programs in their common test cases, we can deduce the following:
<br>
![](/Images/Analysis_6.jpg)
<br>
Using a simple send and receive is always the worst way to calculate the sum of an array. This approach is the slowest of all in all cases, and should not be used for this operation. Using Scatter and Gather gives us much better performance than using simple Send and Receive, but that performance gain is superseded by using Reduce instead of Gather. Using a ring topology gives us better performance that Send and Receive also, but it is worse than Scatter/Gather for smaller workloads. Scatter/Reduce is still better than using a ring in the test cases we conducted.
<br>
![](/Images/Analysis_7.jpg)
<br>
Using a logical topology of a hypercube gives us better performance than simple Send/Receive and Ring, and is at par with Scatter/Gather at smaller workloads. At larger workloads, a hypercube gives us better performance than Scatter/Gather. Compared to Scatter/Reduce, we can see that it performs worse, but extrapolating from these results tells us that as the workload increases, hypercube would become quicker at calculating the sum. A Cartesian topology only gives us real benefits over hypercube when we use 16 nodes in our test cases, and is better than simple Send/Receive, Scatter/Gather and Ring at both 8 and 16 nodes.

The analysis of all the programs shows us that with an increase in the number of elements in the array we start to see a benefit of increasing the number of nodes in the implementation. It also shows us that the Scatter and Reduce methodology of addition is the most efficient across all number of elements and nodes. This is because the Reduce operation needs to communicate less data over the same link as compared to the Gather operation. Reduce operation also needs to allocate much less memory for its operation. A slight caveat to this is that for extremely large clusters or more complex computations, we would see more sophisticated topologies like the ring, hypercube or Cartesian topology be more efficient. In case of an addition operation since the communication overhead can outweigh the computation overhead, we see the Scatter and Reduce approach outperform all.

## **Lessons**

Through these implementations we've analyzed how 6 different algorithms carry out the addition operation on an array of elements. The programs show us how communication occurs across the nodes on various network topologies.
From Program 1 we see that the communication cost of individually sending the sub arrays from one root node and receiving the local sums to the root node, using MPI_Send and MPI_Recv are quite high and hence the program is inefficient across all the node and element counts.

For program 2, using the MPI_Scatter and MPI_Gather methodology, we pass the sub arrays from node 0 using the Scatter method. This scatters the array across the nodes and computes the local sum and the Gather method pools the result into the root node to give us the final total. This proves to be significantly more efficient than the Send and Receive method in program 1. However, with the increase in the number of elements, we see a steep upward trend in the running time of this algorithm as compared to other topologies.

Program 3 implements the Scatter and Reduce methods to carry out the addition. This proves to be the most efficient algorithm for our use case. We see that across all node counts and element sizes this implementation has the lowest run time. The reduce functionality is highly efficient and given that the computation task is minimal for the addition, the reduce method becomes more beneficial than the more refined algorithms.

In Program 4, we use the ring topology to pass the array from one node to its neighboring node to carry out the processing. This reduces the overall communication overhead from one root node and distributes this across all the sub-nodes. The performance benefit of which can be clearly seen as compared to the Send and Receive method. We can also see that with increase in the number of elements, this approach is more fruitful than the Scatter and Gather implementation as well.

The hypercube implementation in Program 5 passes the values in _log k_ steps and is highly efficient compared to the other algorithms. We see that with the increase in the number of elements the time taken by the algorithm keeps reducing as compared to the other algorithms. In the scenario of 8 nodes, we can see its performance match the Scatter and Reduce program. The Cartesian topology proves to be more beneficial in a scenario where the number of nodes is higher. We an improvement in the performance of our algorithm when 16 nodes are involved as opposed to the 8-node implementation of the same. Moreover, with an increase in the elements we see a linear increase in the time for this algorithm while an exponential increase in time for the more rudimentary implementations.
