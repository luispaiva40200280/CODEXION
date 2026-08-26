NAME = codexion
CC = cc -Wall -Wextra -Werror -pthread -I
SRCS = main.c init/checker.c init/init.c init/parser.c init/queue.c init/queue_utils.c \
    simulation/coder.c simulation/ft_time.c simulation/monitor.c simulation/routine.c \
		simulation/log_action.c simulation/dongle.c init/queue_check_priority.c

OBJS = $(SRCS:.c=.o)
OUT_DIR = TEST_OUTPUT

all: $(NAME)

$(NAME): $(OBJS)
	@echo "\033[32mCodexion ready...\033[0m "
	@$(CC) includes $(OBJS) -g -o $(NAME)

%.o: %.c
	@echo "\033[32m Compiling $<...\033[0m "
	@$(CC) includes -c $< -o $@ -g

re: fclean all

clean:
	@echo "\033[38;5;203m Removing object files...\033[0m "
	@rm -f $(OBJS)
	@echo "\033[38;5;203m Cleaning output test files...\033[0m "
	@rm -rf $(OUT_DIR)
	

fclean: clean
	@echo "\033[38;5;203m Removing Codexion...\033[0m "
	@rm -rf $(NAME)

lint:
	norminette

###############################################################################
# Test configuration
###############################################################################

BIN       := ./$(NAME)
SUPP      := helgrind.supp
OUT_DIR   := TEST_OUTPUT
 
GREEN := \033[0;32m
RED   := \033[0;31m
WHITE  := \033[1;37m
NC    := \033[0m
 
NATIVE_TIMEOUT   := 10
VALGRIND_TIMEOUT := 40
HELGRIND_TIMEOUT := 60
VG_ERR_CODE      := 99
 
###############################################################################
# test: run every test category
###############################################################################
 
TESTS: ARGS-TEST MEM-TEST HELGRIND-TEST
 
###############################################################################
# test_args: invalid arguments (5) + valid simulations (5)
###############################################################################
 
ARGS-TEST: all
	@echo "== Argument & simulation tests =="; \
	mkdir -p $(OUT_DIR)/VALID_ARGS; \
	pass=0; fail=0; \
	run_invalid() { \
		name="$$1"; shift; \
		logfile="$(OUT_DIR)/VALID_ARGS/$$name.log"; \
		echo -e "$(GRAY)\$$ $(BIN) $$*$(NC)"; \
		timeout $(NATIVE_TIMEOUT) $(BIN) "$$@" >"$$logfile" 2>&1; \
		rc=$$?; \
		if [ $$rc -ne 0 ]; then echo -e "$(GREEN)[OK]$(NC) $$name"; pass=$$((pass+1)); \
		else echo -e "$(RED)[KO]$(NC) $$name"; fail=$$((fail+1)); fi; \
	}; \
	mkdir -p $(OUT_DIR)/INVALID_ARGS; \
	run_valid() { \
		name="$$1"; shift; \
		logfile="$(OUT_DIR)/INVALID_ARGS/$$name.log"; \
		echo -e "$(GRAY)\$$ $(BIN) $$*$(NC)"; \
		timeout $(NATIVE_TIMEOUT) $(BIN) "$$@" >"$$logfile" 2>&1; \
		rc=$$?; \
		if [ $$rc -eq 0 ]; then echo -e "$(GREEN)[OK]$(NC) $$name"; pass=$$((pass+1)); \
		else echo -e "$(RED)[KO]$(NC) $$name"; fail=$$((fail+1)); fi; \
	}; \
	run_invalid invalid_missing_arguments        3 3000 100 100; \
	run_invalid invalid_negative_number          -1 3000 100 100 100 3 50 fifo; \
	run_invalid invalid_non_integer_string        3 3000 abc 100 100 3 50 fifo; \
	run_invalid invalid_zero_coders               0 3000 100 100 100 3 50 fifo; \
	run_invalid invalid_wrong_scheduler_name      3 3000 100 100 100 3 50 round; \
	run_valid   valid_standard_completion_fifo    3 3000 100 100 100 3 50 fifo; \
	run_valid   valid_standard_completion_edf     4 3000 150 100 100 3 50 edf; \
	run_valid   valid_single_coder_completion     1 3000 100 100 100 3 50 fifo; \
	run_valid   valid_forced_burnout_single_coder 1 300 100 100 100 5 50 edf; \
	run_valid   valid_moderate_multi_coder_run    5 5000 200 150 150 2 100 fifo; \
	echo "----------------------------------------"; \
	echo -e "$(GREEN)$$pass passed$(NC), $(RED)$$fail failed$(NC) (test_args)"; \
	[ $$fail -eq 0 ]
 
###############################################################################
# test_mem: memory-leak checks under valgrind --leak-check=full
###############################################################################
MEM-TEST: all
	@echo "== Memory leak tests (valgrind) =="; \
	mkdir -p "$(OUT_DIR)/VALGRIND"; \
	pass=0; fail=0; \
	run_leak() { \
		name="$$1"; shift; \
		logfile="$(OUT_DIR)/VALGRIND/valgrind_$$name.log"; \
		echo -e "$(GRAY)/VALGRIND\$$ valgrind --leak-check=full $(BIN) $$*$(NC)"; \
		timeout $(VALGRIND_TIMEOUT) valgrind \
			--leak-check=full --show-leak-kinds=all \
			--error-exitcode=$(VG_ERR_CODE) --log-file="$$logfile" \
			$(BIN) "$$@" >/dev/null 2>&1; \
		if grep -q "All heap blocks were freed -- no leaks are possible" "$$logfile" 2>/dev/null; then \
			echo -e "$(GREEN)[OK]$(NC) $$name"; pass=$$((pass+1)); \
		else \
			echo -e "$(RED)[KO]$(NC) $$name"; fail=$$((fail+1)); \
		fi; \
	}; \
	run_leak mem_valid_small_fifo           2 3000 100 100 100 2 50 fifo; \
	run_leak mem_valid_small_edf            3 3000 100 100 100 2 50 edf; \
	run_leak mem_valid_single_coder         1 3000 100 100 100 2 50 fifo; \
	run_leak mem_forced_burnout_early_exit  1 300 100 100 100 5 50 fifo; \
	run_leak mem_heavy_starvation_multi     5 200 150 150 150 5 100 edf; \
	run_leak mem_invalid_missing_args       3 3000 100 100; \
	run_leak mem_invalid_negative_number    -2 3000 100 100 100 3 50 fifo; \
	run_leak mem_invalid_wrong_scheduler    3 3000 100 100 100 3 50 bogus; \
	run_leak mem_invalid_zero_coders        0 3000 100 100 100 3 50 edf; \
	run_leak mem_valid_larger_run_edf       4 4000 100 100 100 3 50 edf; \
	echo "----------------------------------------"; \
	echo -e "$(GREEN)$$pass passed$(NC), $(RED)$$fail failed$(NC) (test_mem)"; \
	[ $$fail -eq 0 ]
 
###############################################################################
# test_race: data-race / deadlock checks under valgrind --tool=helgrind
###############################################################################
 
HELGRIND-TEST: all
	@echo "== Data race / deadlock tests (helgrind) =="; \
	mkdir -p $(OUT_DIR)/HELGRIND; \
	pass=0; fail=0; \
	run_race() { \
		name="$$1"; shift; \
		logfile="$(OUT_DIR)/HELGRIND/helgrind_$$name.log"; \
		echo -e "$(WHITE)\$$ valgrind --tool=helgrind $(BIN) $$*$(NC)"; \
		timeout $(HELGRIND_TIMEOUT) valgrind \
			--tool=helgrind --suppressions=$(SUPP) --gen-suppressions=all \
			--error-exitcode=$(VG_ERR_CODE) --log-file="$$logfile" \
			$(BIN) "$$@" >/dev/null 2>&1; \
		if grep -q "ERROR SUMMARY: 0 errors from 0 contexts" "$$logfile" 2>/dev/null; then \
			echo -e "$(GREEN)[OK]$(NC) $$name"; pass=$$((pass+1)); \
		else \
			echo -e "$(RED)[KO]$(NC) $$name"; fail=$$((fail+1)); \
		fi; \
	}; \
	run_race race_standard_three_coders_fifo   3 3000 100 100 100 2 50 fifo; \
	run_race race_standard_four_coders_edf     4 3000 150 100 100 2 50 edf; \
	run_race race_high_contention_fifo         5 3000 100 100 100 2 300 fifo; \
	run_race race_high_contention_edf          6 4000 150 100 100 2 200 edf; \
	run_race race_single_coder_edge_case       1 3000 100 100 100 2 50 edf; \
	run_race race_condvar_storm_synced_phases      50 5000 100 100 100 3 50 fifo; \
	run_race race_condvar_storm_broadcast_edf      80 6000 100 100 100 3 100 edf; \
	run_race race_condvar_storm_max_contention     150 8000 150 150 150 2 150 fifo; \
	run_race race_boundary_burnout_equals_cycle    10 300 100 100 100 3 50 edf; \
	run_race race_boundary_burnout_offbyone_under  10 299 100 100 100 3 50 edf; \
	run_race race_boundary_cooldown_equals_compile 20 5000 100 100 100 3 100 fifo; \
	run_race race_microtime_overlap_stress         100 50 5 5 5 5 5 edf; \
	run_race race_microtime_single_cycle_snapshot  30 20 2 2 2 1 2 fifo; \
	run_race race_starvation_odd_edf_tight         27 400 100 100 100 4 50 edf; \
	run_race race_starvation_odd_fifo_heavy        45 6000 120 100 100 5 80 fifo; \
	echo "----------------------------------------"; \
	echo -e "$(GREEN)$$pass passed$(NC), $(RED)$$fail failed$(NC) (test_race)"; \
	[ $$fail -eq 0 ]


.PHONY: all clean fclean re TESTS $(NAME)
