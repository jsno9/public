void dfs(vector<vector<int>>& image,int sr,int sc,int newcolor)
{
    int nowcolor=image[sr][sc];
    image[sr][sc]=newcolor;
    int nowrow=image.size();
    //cout<<nowrow  <<endl;
    if(nowrow<1)
        return;
    int nowcol=image[0].size();
    //printf("sr=%d,sc=%d,nowcolor=%d\n",sr,sc,nowcolor);

    if((sr-1>=0) && (image[sr-1][sc]==nowcolor) )
        dfs(image,sr-1,sc,newcolor);
    if((sr+1<nowrow) && (image[sr+1][sc]==nowcolor))
        dfs(image,sr+1,sc,newcolor);
    if((sc-1>=0) && (image[sr][sc-1]==nowcolor))
        dfs(image,sr,sc-1,newcolor);
    if((sc+1<nowcol) && (image[sr][sc+1]==nowcolor))
        dfs(image,sr,sc+1,newcolor);
                                                    
    return;
                                                        }
