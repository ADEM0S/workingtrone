typedef struct DYNAMIC_ARRAY_STRUCT
{
	Video *array;
	size_t used;
	size_t size;
} DynArray;

void init_array(DynArray *a, size_t initial_size)
{
	a->array = (Video*) malloc(initial_size * sizeof(VIDEO_STRUCT));
	a->used = 0;
	a->size = initial_size;
}

void insert_array(DynArray *a, Video element)
{
	if (a->used == a->size)
	{
		a->size *= 2;
		a->array = (Video*) realloc(a->array, a->size * sizeof(VIDEO_STRUCT));
	}
	a->array[a->used++] = element;
}

void free_array(DynArray *a)
{
	free(a->array);
	a->array = NULL;
	a->used = a->size = 0;
}
