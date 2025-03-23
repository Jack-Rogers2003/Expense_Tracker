#include "expensetracker.h"
#include "category.h"

int main() {
    ExpenseTracker x;
    x.addCategory(Category("test"));
    x.getCategory("test");
    return 0;
}