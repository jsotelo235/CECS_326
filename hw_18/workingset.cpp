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
  /* This is the header line as shown in the homework assignment */
  printf("Time \t Size \t  Set\n");

  for(int time = w-1; time < n; time++)
  {
    std::vector<int> vec;
    for(int i = time; i < time+w; i++)
    {
      if( (std::find(vec.begin(), vec.end(), pages[i])) != vec.end() )
      {
        // do nothing
      }
      else
      {
	if(i < n)
	 vec.push_back(pages[i]);
      }
    }

   printf("%d \t %d \t ", time, (int)vec.size());

   for(int i = 0; i < vec.size(); i++)
   {
     printf(" %d", vec.at(i));
   }
   printf("\n");
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
