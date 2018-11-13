#include <stdio.h>
#include <unistd.h>
#include <iostream>	// std::cout
#include <vector>	// std::vector
#include <algorithm>	// std::find

/*
 n is the array size, w is the window size and pages is the list
 of pages.
*/
void workingset(int n, int w, int pages[])
{
  std::vector<int> vec;

  /* This is the header line as shown in the homework assignment */
  printf("Time \t Size \t Set\n");

  /* DUMMY SAMPLE CODE, REMOVE! */
  /* This print shows how to compute the loop bounds. */
  //printf("Do from %d to %d\n",w-1,n-1);

  /* This print confirms the parameter pass, it shows you how to line
    things up with the headers.*/
  printf("%d \t %d \t ", (int)vec.size(), w);

  for(int time= w-1; time < n; time++)
  {
    for(int i = 0; i < time; i++)
    {
      // using iterator to store the position of the searched element
      std::vector<int>::iterator findVec;
      
      // using std::find to find the element in the given range of numbers 
      findVec = std::find(vec.begin(), vec.end(), pages[time]);

      if( findVec != vec.end() )
      {
        // do nothing
      }
      else
      {
        vec.push_back(pages[time]);
      }      
    }
  }

  for(int i = 0; i <  w; i++)
  {
    std::cout << " " << vec.at(i);
  }
  printf("\n");
};

int main()
{
  int list1[] = {1,2,3,3,2,3,2,1,2,1,2,1,3,1,3,1,2,3,1};
  int list2[] = {1,2,3,3,4,4,5,3,3,2,1,2,1,2,3,4,5,3,2,4,2,4,2,2};
  /* notice the tricky way of getting the number of integers in the array */

  workingset(sizeof(list1)/sizeof(int),3,list1);
  workingset(sizeof(list2)/sizeof(int),4,list2);

  return 0;
};
