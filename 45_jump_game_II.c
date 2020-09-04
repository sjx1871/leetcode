struct step_store
{
    int idx;
    int num;
    int min_step;
};

int get_step(int *nums, int cur, int max, struct step_store *step)
{
    int res = -1;

    if (cur > max)
        return 0;

    if (cur == max || max == 0)
    {
        res = 0;
        goto back;
    }
    //printf("start cur = %d, nums[cur] = %d, \n", cur, nums[cur]);
    if (nums[cur] == 0 && cur < max)
    {
        res = -1;
        goto back;
    }

    //printf("step[%d].min_step = %d, \n", cur, step[cur].min_step);
    if (step[cur].min_step != 0)
        return step[cur].min_step;

    int *array = malloc(nums[cur] * sizeof(int));
    if (!array)
        return -1;
    
    for (int i = nums[cur] - 1; i >= 0; i--)
    {
        int ret = get_step(nums, cur + i + 1, max, step);
        if (ret < 0)
            array[i] = -1;
        else
            array[i] = ret + 1;
        //printf("--%s,%d nums[%d] = %d, goto step = %d, array[%d] = %d\n", __func__, __LINE__, cur, nums[cur], i+1, i, array[i]);
    }

    res = array[0];
    for (int i = 0; i < nums[cur]; i++)
    {
        if (array[i] <= 0)
            continue;
        
        if (res <= 0 || array[i] < res)
            res = array[i];
    }
    
    //printf("%s,%d nums[%d] = %d, res = %d\n", __func__, __LINE__, cur, nums[cur], res);
    free(array);

back:
    //printf("+++step[%d].min_step = %d, \n", cur, step[cur].min_step);
    step[cur].min_step = res;
    return res;
}

int jump(int* nums, int numsSize){
    //1 遍历所有节点，按照数值生成多叉树的节点。最后取树的最短叶节点。
    //2 递归处理，用当前数组的值，计算所有可能的剩余步数，最后取最小值；最终条件为idx=numSize-1时，return 1;
    //3 该问题需要动态规划吗？好像贪心就可了。找到覆盖范围内的最后一个最大值就行

    //2 递归处理，使用临时变量存储每一个节点所需的步数。当数据为25002个时，就超时了，每个点只计算一次都会超时
    #if 0
    struct step_store *step = malloc(sizeof(struct step_store) * numsSize);
    memset(step, 0, sizeof(struct step_store) * numsSize);
    //printf("numsSize = %d\n", numsSize);
    int ret = get_step(nums, 0, numsSize - 1, step);
    //for (int i = 0; i < numsSize; i++)
    {
        //printf("step[%d].min_step = %d, \n", i, step[i].min_step);
    }
    return ret;
    #endif

    /*3 最大长度覆盖。时间复杂度O(n^2)？？最坏情况[1,1,1,1,1,1,1,1,1,1]*/
    #if 0
    int step = 0;
    int i = 0;

    while(i < numsSize)
    {
        if (i == numsSize - 1)  //当i == numsSize - 1时，i已经到最大了，不必再继续下去，同时不必step++
            break;
        
        if (i + nums[i] >= numsSize - 1) //当刚好i + nums[i] == numsSize-1时，说明step再加一步就能到底了
        {
            step++;
            break;
        }
        
        int next_max_num = 0;
        int j = i + 1;
        int idx = 0;
        
        /*10,1,8,1,1,1,1,1,1,1,1....在10的范围内8是最大值，步数就是10-8-1一共两步，但这明显不是最优解，因为10-1才是，只需一步。
        这就说明在当前值覆盖范围内，最大值并非最优解，最优解其实是最大值的i+nums[i]。
        为何呢？因为i+nums[i]所能到的最远位置一定大于等于最大值所在的j+nums[j]，那就决定了i+nums[i]一定比j+nums[j]走的要远。
        当“每一步都能走的越远”最终才能达到最少步数的目的。*/
        for (j; j <= nums[i] + i && j < numsSize; j++)
        {
            if (next_max_num <= nums[j] + j)  //取最后一个最大值
            {
                next_max_num = nums[j] + j;
                idx = j;
            }
        }
        
        //printf("max num is nums[%d] = %d\n", idx, nums[idx]);

        //下面的这个if判断，直接造成4ms的运行时间差。为啥啊
		//这里的if可以删掉，是因为要走到低，就必须能找到一个最大值
        //if (idx != 0)
            step++;
        
        i = idx;
    }
asd
    return step;
    #endif

    /*官方解法，时间复杂度O(n)。高明在for遍历每一个节点，不仅找到了最大值，还不会出现重复访问节点(因为下一个最佳值一定不在当前nums[i]覆盖范围内，那么就可以不用再度访问一次)，所以时间很短O(n)。
    而自己的解法一个是条件多，一个是每次for循环查找下一个最大值一个有重复访问i的问题*/
    int step = 0;
    int max = 0;
    int end = 0;

    //不必访问最后一个元素，因为第一跳，跳到nums[0]时就加过了一次，所以最后一个值不必取访问
    for (int i = 0; i < numsSize - 1; i++)
    {
        if (max <= i + nums[i])
        {
            max = i + nums[i];
            /*官方解法优化一步1，节省4ms*/
            /*if (i + nums[i] >= numsSize - 1)
            {
                step++;
                break;
            }*/
        }


        /*当i == end时，表示当前已经到达了i+nums[i]的最大访问边界，这时这就要更新end为i到nums[i]范围内的最大值max，并且step++*/
        /*自所以要在i == end时让step++，这是因为已经访问到了最大边界。*/
        /*而end = max，一是因为范围边界到了，已经更新了最大值max，也就是已经确定了新的边界位置。
        二是因为下一个将得到的max一定不在i+nums[i]范围内，这个逻辑怎么来的呢？*/
        if (i == end)
        {
            end = max;
            step++;
            /*官方解法优化一步2，节省4ms*/
            if (i + nums[i] >= numsSize - 1) return step;
        }
        
        
    }
    return step;
}