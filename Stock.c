typedef struct Stock {
	struct Stock *previous;
	struct Stock *next;
} Stock;
typedef struct MemoryPool {
    unsigned int entries;
    unsigned int stocks;
    struct Stock *first;
    struct Stock *last;
} MemoryPool;
static MemoryPool *Stockpool = NULL;
void  Stockinit(){
    Stockpool = malloc(sizeof(MemoryPool));
    if (Stockpool==NULL) { printf("Failed to allocate MemoryPool\n"); return; }
    Stockpool->entries = 0;
    Stockpool->stocks = 0;
    if (Stockpool != NULL) {
        Stockpool->first = NULL;
        Stockpool->last = NULL;
        //make it safe, we allocate the first one, which provides a valid pointer.
        Stockpool->last = (struct Stock *)malloc(sizeof(struct Stock)); //guarantee we have a valid pointer
        Stockpool->last->previous = NULL; //marks it as first
        Stockpool->stocks++;
        Stockpool->first = Stockpool->last;
    }
}
void StockbuffGrow(size_t size){
    struct Stock *old = Stockpool->last;
    Stockpool->last = (struct Stock *)malloc(size);
    Stockpool->last->previous = old;
    Stockpool->stocks++;
}
void Stockadd(size_t size, void *stock){
    if ( Stockpool->entries >= Stockpool->stocks ) StockbuffGrow(size);
    struct Stock *oldprevious = Stockpool->last->previous;
    memcpy(Stockpool->last, stock, size);
    Stockpool->last->previous = oldprevious;
    Stockpool->entries++;
}
void *getLastStock(void){
    return Stockpool->last;
}
void *newStock(size_t size, void *stock){
    Stockadd(size, stock);
    return getLastStock();
}
void releaseStock(void* item) {
    if (item == NULL || Stockpool == NULL) return;
    struct Stock *stock = (struct Stock *)item;
    if (stock->previous != NULL) stock->previous->next = stock->next; //close the gap
    if (stock->next != NULL) stock->next->previous = stock->previous; //reconnect to next, can also be NULL (dead end)
    free(stock);
    Stockpool->entries--;
    Stockpool->stocks--;
}
void freeStockbuff() {
    for (struct Stock *pre = Stockpool->last; pre!=NULL; pre = pre->previous) free(pre);
    Stockpool->stocks = 0;
    Stockpool->entries = 0;
    free(Stockpool);
    Stockpool = NULL;
}