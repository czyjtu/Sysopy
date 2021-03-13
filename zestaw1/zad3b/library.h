struct table{
    int size;
    int num_blocks;
    struct block **blocks;
};

struct block{
    int size;
    int num_lines;
    int idx;
    char **lines;
};

struct table* create_table(int num_blocks);

struct block* create_block(char* file_name);

void merge_files(int num_files, char** files_list, struct table* table);

void remove_block(struct table* table, int idx);

void add_block(struct table* table, struct block* block);

void remove_row(struct table* table, int block_idx, int row_idx);

void print_block(struct table* table, int block_idx);

void print_files(struct table* table);

void clear(struct table* table);