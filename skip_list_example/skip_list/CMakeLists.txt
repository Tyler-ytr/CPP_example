# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_LIB_SRCS)
set(SkipListHeaders ${DIR_LIB_SRCS})
# 指定生成 SkipList 链接库
# add_library (SkipList ${DIR_LIB_SRCS})