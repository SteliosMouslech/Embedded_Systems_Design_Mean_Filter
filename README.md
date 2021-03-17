
# Embedded Systems Design: Mean Filter
A repository containing the project done during the 9th semester university subject "Embedded Systems Design"


### Team Members
- [Stelios Mouslech](https://github.com/SteliosMouslech)
- [Nina Lazaridou](https://github.com/nlazaridou)
### About Project
In this project we implement a  **Mean Filter** for Image Processing using a given pseudocode on the **ARM7TDMI** Processor using the **ARMulator** environment. Afterwards we implement different code transformations and test different memory  hierarchies. For these implementations we measure important execution time and memory size metrics.

The project is split into 3 parts:

## Part 1 - Algorithm Implementation, Optimizations and Loop Transformations
In the first part of the project we assume ideal memory and we
- Implement the **Mean Filter** given a pseudocode
- Test Different logical algorithmic optimizations  (e.g  **Integral Image** ) 
- Test Different **Loop transformations** (e.g Loop Unroll, Loop Inversion,Loop Tiling)
- Get execution time metrics for all the above implementations using AXD Debugger

<img src="https://github.com/SteliosMouslech/Embedded_Systems_Design_Median_Filter/blob/main/part1_readme.PNG" width="800" height="500">


## Part 2 - Memory Hierarchy
In this part using the best version from the previous part we implement different memory hierarchies and measure the trade-off between memory size/speed and execution time

<img src="https://github.com/SteliosMouslech/Embedded_Systems_Design_Median_Filter/blob/main/part2_readme.png" width="800" height="500">

## Part 3 - Data Reusability

In the final Part of the project we implement a very fast "Cache-like" memory on the previous version and try to reduce the **Total Execution Cycles** of our program using data reusability techniques. We also try to reduce the "overhead" of adding our data reusability method
