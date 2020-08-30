#define UINT32 unsigned int
#define UINT8 unsigned char

#define TYPE_SIZE sizeof(UINT32)

int findDuplicate(int* nums, int numsSize){
    /*1 bit map*/
    /*
    UINT32 bit[30000];// = (unsigned char *)malloc(size);

    memset(bit, 0, sizeof(bit));
    for(int i = 0; i < numsSize; i++)
    {
        UINT32 flag = 1 << (nums[i] % TYPE_SIZE);
        
        if ((bit[nums[i]/TYPE_SIZE] & flag) == flag)
        {
            return nums[i];
        }
        else
        {
            bit[nums[i]/TYPE_SIZE] = bit[nums[i]/TYPE_SIZE] | flag;
        }
        printf(" nums[%d] = %d, flag = %x, bit[] = %x\n", i, nums[i], flag, bit[nums[i]/TYPE_SIZE]);
    }
    return -1;
    */

    /*2 快慢指针法，使用值来计算*/
#if 0
    /*初始值都从0开始*/
    int *fast = &nums[0];
    int *slow = &nums[0];
    int miss_idx = 0;
    int repeat_num = 0;

    /*查找相遇的点法1*/
    /*slow = &nums[*slow];
    fast = &nums[*fast];
    fast = &nums[*fast];

    while(slow != fast)
    {
        //slow走一步，fast走两步
        slow = &nums[*slow];
        fast = &nums[*fast];
        fast = &nums[*fast];
        
        ++miss_idx;
    }*/
    /*查找相遇的点法2*/
    while(1)
    {
        if (slow != &nums[0] && slow == fast)
            break;
        //slow走一步，fast走两步
        slow = &nums[*slow];
        fast = &nums[*fast];
        fast = &nums[*fast];
        
        ++miss_idx;
    }

    if (miss_idx == numsSize)
    {
        printf("overflow\n");
        return -1;
    }

    /*慢指针从0开始*/
    slow = &nums[0];
    while(slow != fast)
    {   
        //printf("slow = %p, *slow = %d fast = %p *fast = %d\n", slow, *slow, fast, *fast);
        
        /*快慢指针都移动一步*/
        slow = &nums[*slow];
        fast = &nums[*fast];
        repeat_num++;
    }

    /*该指针指向的位置的下标即为要获取的结果*/
    return ((void*)slow - (void*)&nums[0])/sizeof(int);//slow - &nums[0];
#endif
    
    /*3 快慢指针法，使用下标来计算*/
    int fast = 0;
    int slow = 0;

    while(1)
    {
        if (slow != 0 && slow == fast)
        {
            break;
        }
        slow = nums[slow];
        fast = nums[nums[fast]];
    }

    slow = 0;
    while(slow != fast)
    {
        slow = nums[slow];
        fast = nums[fast];
    }

    return slow; //or return fast;
}