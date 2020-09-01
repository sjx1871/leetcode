void sortColors(int* nums, int numsSize){
    int p0 = 0;
    int cur = 0;
    int p2 = numsSize - 1;

    /*先找到不为0的p0，和不为2的p2，然后于cur交换*/
    /*很遗憾，代码判断条件太麻烦，垃圾的我没写通*/
    /* 过不了 [2,0]
    while (cur <= p2 && (p0 < (numsSize - 1)) && p2 > p0)
    {
        while ((p0 < (numsSize - 1)) && nums[p0] == 0) {p0++;} //找到第一个不为0的p0
        while (p2 > 1 && nums[p2] == 2) {p2--;} //找到第一个不为2的p2
        
        if (p0 > cur)
            cur = p0;
            
        if (nums[cur] == 0)
        {
            nums[cur] = nums[p0];
            nums[p0++] = 0;
            if (nums[cur] == 1)
                cur++;
        }
        else if (nums[cur] == 2)
        {
            nums[cur] = nums[p2];
            nums[p2--] = 2;
            if (nums[cur] == 1)
                cur++;
        }

        if (cur <= p2 && nums[cur] == 1)
            cur++;
        
        printf("p0 = %d, cur = %d, p2 = %d\n", p0, cur, p2);
        for (int i = 0 ; i < numsSize; i++)
            printf("%d ", nums[i]);
        printf("\n");
    }*/

    while (cur <= p2)
    {
        if (nums[cur] == 0)
        {
            nums[cur++] = nums[p0];  //这里cur左边一定全部是0或者1，不可能出现2，因为左边都是交换0过的值，所以可以cur++
            nums[p0++] = 0;
        }
        else if (nums[cur] == 2)
        {
            nums[cur] = nums[p2];   //这里的右边换过来的值可能是2，所有cur不能++
            nums[p2--] = 2;
        }
        else
            cur++;                  //等于1时，cur++
    }
}