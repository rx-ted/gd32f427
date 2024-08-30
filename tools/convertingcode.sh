###
 # @Author: rx-ted 1552488649@qq.com
 # @Date: 2024-08-30 13:49:52
 # @LastEditors: rx-ted 1552488649@qq.com
 # @LastEditTime: 2024-08-30 13:51:03
 # @Description: 
 # Copyright (c) 2024 by rx-ted, All Rights Reserved. 
### 
 

find .  -name "*.h" -print0 |  xargs -0 -I {} bash -c 'iconv -f gb2312 -t utf8 "{}" > "{}.tmp" && mv "{}.tmp" "{}"'
echo 'Failed to generate tmp files:'
find . -name "*.tmp"

