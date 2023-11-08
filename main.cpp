#include "minilog.h"

int main() {
	/* Test0 */
	fmt::print("/{:*^30}/\n","Test0");
	minilog::set_log_level(minilog::log_level::debug);
	minilog::log_debug("{:_^30}", "debug");
	minilog::log_info("{:_^30}", "info");
	minilog::log_critical("{:_^30}", "critical");
	minilog::log_error("{:_^30}", "error");
	minilog::log_fatal("{:_^30}", "fatal");
	/* Test1 */
	fmt::print("/{:*^30}/\n","Test1");
	int n = 42;
	MINILOG_P(n);
	return 0;
}
