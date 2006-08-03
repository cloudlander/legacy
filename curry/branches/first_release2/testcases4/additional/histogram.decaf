// amin-gopals, CS143 Winter 2000-01

void PrintNum(int number, int digits, bool printNum)
{
  int maxNum;
  int tmp;

  if (!printNum) {
    // Reserve a space for '-'
    Print(" ");
    while (digits > 0) {
      Print(" ");
      digits = digits - 1;
    }
    return;
  }
  

  maxNum = 1;
  
  while (digits > 1) {
    maxNum = maxNum * 10;
    digits = digits -1;
  }
  

  if (number < 0) {
    tmp = -number;
  } else {
    if (number == 0) tmp = 1;
    else             tmp = number;
    Print(" ");
  }
  
  while (tmp < maxNum) {
    tmp = tmp*10;
    Print(" ");
  }
  
  Print(number);
}      


int[] ReadArray(int numScores, int minScore, int maxScore)
{
  int i;
  int num;
  int [] arr;
  
  arr = NewArray(numScores, int);
  i = 0;
  while (i < arr.length()) {
    Print("Enter next number: ");
    num = ReadInteger();
    
    if (num < minScore || num > maxScore) {

      Print("\n*** Score not within previous boundaries: [",
        minScore, ":", maxScore, "]\n");
        
      Print("*** Please try again...\n\n");
    } else {
      arr[i] = num;
      i = i + 1;
    }
  }
  return arr;
}

void Sort(int []arr)
{
  int i;
  int j;
  int val;

  i = 1;
  while (i < arr.length()) {
    j = i -1;
    val = arr[i];
    while (j >= 0) {
      if (val >= arr[j]) break;
      arr[j+1] = arr[j];
      j = j -1;
   }
   arr[j+1] = val;
   i = i + 1;
  }
}

void PrintBuckets(int []arr, int numBuckets, int min, int max)
{
  int i;
  int bucket;
  int difference;
  int[] buckets;
  int currBucket;
  int arrayLength;
  int firstBucketPos;
  int j;
	buckets = NewArray(numBuckets, int);
  
  difference = max - min;
  i = 0;
  
  for (i = 0; i < numBuckets; i = i + 1)
    buckets[i] = 0;
      

  currBucket = 0;
  

  arrayLength = arr.length();
  
  for (i=0; i < arrayLength; i = i+1) {
    // Adjust currBucket so it is appropriate for this value
    while (currBucket < buckets.length()) {
      if ( arr[i] <= ( min + difference*(currBucket+1)/numBuckets ) )
        break;
      currBucket = currBucket + 1;
    }
    
    // Add the value to the current bucket
    buckets[currBucket] = buckets[currBucket] + 1;
  }


  firstBucketPos = (difference/numBuckets)/2;

  Print("Please note:\n");
  Print("------------\n");
  Print("Bucket Extent is +-", firstBucketPos, ".\n");
  Print("Two characters in height corresponds to one value in the bucket.\n");
  Print("Disclaimer: There may be rounding error due to lack of ",
    "floating point support!\n\n");

  Print("Histogram:\n\n");
  
  for (i = buckets.length()-1; i >=0; i = i-1) {
    int upperBound;
    int bucketPos;
    int j;
    int height;
	upperBound = min + difference*(i+1)/numBuckets;


    bucketPos = min + firstBucketPos + difference*i/numBuckets;

    // Print the dashes above

    PrintNum(upperBound, 6, true);
	Print(" |");
    

    height = buckets[i];
    if (i != buckets.length()-1)
      if (buckets[i+1] > height)
        height = buckets[i+1];
        
    for (j = 0; j < height; j = j+1)
      Print("--");
    Print("\n");
    
    // Print the spaces and the | for the bin
    PrintNum(bucketPos, 6, false);
	Print(" |");
    for (j = 0; j < buckets[i]; j = j+1)
      Print("  ");
    if (buckets[i] != 0)
      Print("|");
    Print("\n");
  }


  PrintNum(min, 6, true);
  Print(" |");
  for (j = 0; j < buckets[0]; j = j+1)
    Print("--");
  Print("\n");
    
  Print("\n");
}


void main()
{
  int[] arr;
  int numScores;
  int numBuckets;
  int minScore;
  int maxScore;

  Print("\nHistogram by Andrew Min and Gopal Santhanam (Winter 2001).\n\n");
  
  Print("This program will read in a bunch of numbers and produce\n");
  Print("a crude histogram of the results in bucket sizes of your\n");
  Print("choosing.  Number of buckets should divide evenly into the\n");
  Print("range.\n\n");

  Print("How many scores? ");
  numScores = ReadInteger();
  
  if (numScores == 0) {
    Print("No point in analyzing zero scores!  Exiting...\n");
    return;
  } else if (numScores < 0) {
    Print("Must enter a non-negative number!  Exiting...\n");
    return;
  }
  
  Print("How many buckets? ");
  numBuckets = ReadInteger();
  
  if (numBuckets <= 0) {
    Print("*** Must enter a postivie number of buckets!  Exiting...\n");
    return;
  }
  
  Print("Min score? ");
  minScore = ReadInteger();
  
  Print("Max score? ");
  maxScore = ReadInteger();
  
  if (maxScore <= minScore) {
    Print("*** Max score must be greater than minimum score!  Exiting...\n");
    return;
  }
  
  arr = ReadArray(numScores, minScore, maxScore);

  // If the array length is zero we know we had an error.  Just quit...
  if (arr.length() == 0)
     return;
     
  Sort(arr);

  Print("\n\n");
  PrintBuckets(arr, numBuckets, minScore, maxScore);
}
