// you can write to stdout for debugging purposes, e.g.
// printf("this is a debug message\n");

int solution(int A[], int N) {
    int idx = 0;
    int left_sum = 0;
    int right_sum = 0;
    int P = 0;
    
    // First loop - Find right side sum starting with A[]1
    for (idx=1; idx<N; idx++)
    {
        right_sum += A[idx];
    }
    
    // If P == 0, the left_sum is also 0.  See if we have 
    // a match
    if (right_sum == 0)
    {
        return 0;
    }
    
    // Now iterate from the left to right and look for an equilibrim
    // index.  P starts at 1 in this case
    for(idx=0, P=1; idx<N-1 && P<N; idx++, P++)
    {
        left_sum += A[idx];
        right_sum -= A[P];
        if (left_sum == right_sum)
        {
            return P;
        }
    }
        
    // No equilibrim index found so return -1
    return -1;
}
