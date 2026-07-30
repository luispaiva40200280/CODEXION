#include "codexion.h"
#include <stdio.h>

/* 
 * Ensure your queue functions (init_queue, compare_priority, 
 * ft_heappush, heap_shift_down, swap_coders, ft_heappop) are included here 
 * or compiled alongside this file.
 */

int main(void)
{
    t_data data;

    // 1. Setup Dummy Rules
    data.number_of_coders = 3;
    data.time_to_burnout = 100;
    
    // Set to whatever integer/macro you use for FIFO
    // If EDF is 1, let's assume FIFO is 0.
    data.scheduler = FIFO; 

    // Initialize the queue
    if (init_queue(&data) != 0)
    {
        printf("Queue initialization failed.\n");
        return (1);
    }

    // 2. Setup Dummy Coders
    t_coder coder1, coder2, coder3;

    coder1.id = 1;
    coder1.request_time = 500;
    coder1.last_compile_start = 10;
    coder2.id = 2;
    coder2.request_time = 100; // Lowest time -> Should be FIRST in FIFO
    coder2.last_compile_start = 100;
    coder3.id = 3;
    coder3.request_time = 300;
    coder3.last_compile_start = 150;

    // 3. Push Coders (Intentionally pushed out of order)
    printf("Pushing coders in order: 1, 2, 3\n");
    ft_heappush(&data, &coder1);
    ft_heappush(&data, &coder2);
    ft_heappush(&data, &coder3);

    // 4. Extract and Validate
    printf("Expected %i Extraction: 2 -> 3 -> 1\n", data.scheduler);
    printf("Actual Extraction:        ");
    
    t_coder *popped_coder;
    while ((popped_coder = ft_heappop(data.queue)) != NULL)
    {
        printf("%d -> ", popped_coder->id);
    }
    printf("NULL\n");

    // 5. Cleanup (Free the memory allocated in init_queue)
    free(data.queue->coders);
    free(data.queue);

    return (0);
}
