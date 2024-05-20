/*c: sayem 088
20 May 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 100
#define MAX_MENU_ITEMS 50
#define MAX_ORDERS 50
#define MAX_RESERVATIONS 50
#define MAX_CUSTOMERS 50

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"


struct menuItem {
    int id;
    char name[MAX_NAME_LENGTH];
    float price;
    int quantity;
};

struct order {
    int orderId;
    int customerId;
    int menuItemId;
    int quantity;
};

struct reservation {
    int reservationId;
    int customerId;
    char date[11];
    char time[6];
    int numberOfPeople;
};

struct customer {
    int id;
    char name[MAX_NAME_LENGTH];
    char contact[MAX_NAME_LENGTH];
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
};

void customerMenu(struct menuItem menu[], int menuCount, struct order orders[], int *orderCount, struct reservation reservations[], int *reservationCount, struct customer customers[], int *customerCount);
void adminMenu(struct menuItem menu[], int *menuCount, struct order orders[], int *orderCount);
void addMenuItem(struct menuItem menu[], int *count);
void searchMenuItem(struct menuItem menu[], int count);
void updateMenuItem(struct menuItem menu[], int count);
void deleteMenuItem(struct menuItem menu[], int *count);
void listMenuItems(struct menuItem menu[], int count);
void takeOrder(struct order orders[], int *orderCount, struct customer customers[], int customerCount, struct menuItem menu[], int menuCount);
void viewOrders(struct order orders[], int count);
void deleteOrder(struct order orders[], int *orderCount);
void addReservation(struct reservation reservations[], int *reservationCount, struct customer customers[], int customerCount);
void viewReservations(struct reservation reservations[], int count);
void deleteReservation(struct reservation reservations[], int *count);
void addCustomer(struct customer customers[], int *count);
int customerLogin(struct customer customers[], int count);
void customerSignup(struct customer customers[], int *count);


int main() {
    struct menuItem menu[MAX_MENU_ITEMS];
    struct order orders[MAX_ORDERS];
    struct reservation reservations[MAX_RESERVATIONS];
    struct customer customers[MAX_CUSTOMERS];

    int menuCount = 0;
    int orderCount = 0;
    int reservationCount = 0;
    int customerCount = 0;

    int choice;

    while (1) {
        printf("\nRestaurant Management System\n");
        printf("1. Customer\n");
        printf("2. Admin\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                customerMenu(menu, menuCount, orders, &orderCount, reservations, &reservationCount, customers, &customerCount);
                break;
            case 2:
                adminMenu(menu, &menuCount, orders, &orderCount);
                break;
            case 3:
                printf("\nExiting system...\n");
                exit(0);
            default:
                printf(ANSI_COLOR_RED "\nInvalid choice. Please try again.\n" ANSI_COLOR_RESET);
        }
    }

    return 0;
}


void customerMenu(struct menuItem menu[], int menuCount, struct order orders[], int *orderCount, struct reservation reservations[], int *reservationCount, struct customer customers[], int *customerCount) {
    int choice;
    int customerId = -1;

    printf("\nCustomer Menu\n");
    printf("1. Login\n");
    printf("2. Signup\n");
    printf("3. Back\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            customerId = customerLogin(customers, *customerCount);
            break;
        case 2:
            customerSignup(customers, customerCount);
            break;
        case 3:
            return;
        default:
            printf(ANSI_COLOR_RED "\nInvalid choice. Please try again.\n" ANSI_COLOR_RESET);
            return;
    }

    if (customerId != -1) {
        while (1) {
            printf("\nCustomer Actions\n");
            printf("1. Search Menu Item\n");
            printf("2. Order Food\n");
            printf("3. View Reservations\n");
            printf("4. Cancel Reservation\n");
            printf("5. Logout\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    searchMenuItem(menu, menuCount);
                    break;
                case 2:
                    takeOrder(orders, orderCount, customers, *customerCount, menu, menuCount);
                    break;
                case 3:
                    viewReservations(reservations, *reservationCount);
                    break;
                case 4:
                    deleteReservation(reservations, reservationCount);
                    break;
                case 5:
                    return;
                default:
                    printf(ANSI_COLOR_RED "\nInvalid choice. Please try again.\n" ANSI_COLOR_RESET);
            }
        }
    }
}


void adminMenu(struct menuItem menu[], int *menuCount, struct order orders[], int *orderCount) {
    int choice;

    while (1) {
        printf("\nAdmin Menu\n");
        printf("1. Add Menu Item\n");
        printf("2. Delete Menu Item\n");
        printf("3. List Menu Items\n");
        printf("4. Take Order\n");
        printf("5. View Orders\n");
        printf("6. Delete Order\n");
        printf("7. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addMenuItem(menu, menuCount);
                break;
            case 2:
                deleteMenuItem(menu, menuCount);
                break;
            case 3:
                listMenuItems(menu, *menuCount);
                break;
            case 4:
                takeOrder(orders, orderCount, NULL, 0, menu, *menuCount);
                break;
            case 5:
                viewOrders(orders, *orderCount);
                break;
            case 6:
                deleteOrder(orders, orderCount);
                break;
            case 7:
                return;
            default:
                printf(ANSI_COLOR_RED "\nInvalid choice. Please try again.\n" ANSI_COLOR_RESET);
        }
    }
}


void addMenuItem(struct menuItem menu[], int *count) {
    if (*count >= MAX_MENU_ITEMS) {
        printf(ANSI_COLOR_RED "\nMenu item limit reached.\n" ANSI_COLOR_RESET);
        return;
    }
    menu[*count].id = *count + 1;
    printf("\nEnter name: ");
    scanf("%s", menu[*count].name);
    printf("Enter price: ");
    scanf("%f", &menu[*count].price);
    printf("Enter quantity: ");
    scanf("%d", &menu[*count].quantity);
    (*count)++;
    printf(ANSI_COLOR_GREEN "\nMenu item added successfully.\n" ANSI_COLOR_RESET);
}


void searchMenuItem(struct menuItem menu[], int count) {
    char searchName[MAX_NAME_LENGTH];
    printf("\nEnter name of the item to search: ");
    scanf("%s", searchName);

    for (int i = 0; i < count; i++) {
        if (strcmp(menu[i].name, searchName) == 0) {
            printf(ANSI_COLOR_GREEN "\nItem found: %s, Price: %.2f, Quantity: %d\n" ANSI_COLOR_RESET, menu[i].name, menu[i].price, menu[i].quantity);
            return;
        }
    }
    printf(ANSI_COLOR_RED "\nItem not found.\n" ANSI_COLOR_RESET);
}


void updateMenuItem(struct menuItem menu[], int count) {
    int id;
    printf("\nEnter ID of the item to update: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (menu[i].id == id) {
            printf("Enter new name: ");
            scanf("%s", menu[i].name);
            printf("Enter new price: ");
            scanf("%f", &menu[i].price);
            printf("Enter new quantity: ");
            scanf("%d", &menu[i].quantity);
            printf(ANSI_COLOR_GREEN "\nMenu item updated successfully.\n" ANSI_COLOR_RESET);
            return;
        }
    }
    printf(ANSI_COLOR_RED "\nItem not found.\n" ANSI_COLOR_RESET);
}


void deleteMenuItem(struct menuItem menu[], int *count) {
    int id;
    printf("\nEnter ID of the item to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < *count; i++) {
        if (menu[i].id == id) {
            for (int j = i; j < *count - 1; j++) {
                menu[j] = menu[j + 1];
            }
            (*count)--;
            printf(ANSI_COLOR_GREEN "\nMenu item deleted successfully.\n" ANSI_COLOR_RESET);
            return;
        }
    }
    printf(ANSI_COLOR_RED "\nItem not found.\n" ANSI_COLOR_RESET);
}

void listMenuItems(struct menuItem menu[], int count) {
    printf("\nMenu Items:\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d, Name: %s, Price: %.2f, Quantity: %d\n", menu[i].id, menu[i].name, menu[i].price, menu[i].quantity);
    }
}

void takeOrder(struct order orders[], int *orderCount, struct customer customers[], int customerCount, struct menuItem menu[], int menuCount) {
    if (*orderCount >= MAX_ORDERS) {
        printf(ANSI_COLOR_RED "\nOrder limit reached.\n" ANSI_COLOR_RESET);
        return;
    }

    int customerId;
    if (customers != NULL) {
        printf("\nEnter your customer ID: ");
        scanf("%d", &customerId);
        int customerFound = 0;
        for (int i = 0; i < customerCount; i++) {
            if (customers[i].id == customerId) {
                customerFound = 1;
                break;
            }
        }
        if (!customerFound) {
            printf(ANSI_COLOR_RED "\nCustomer not found.\n" ANSI_COLOR_RESET);
            return;
        }
    } else {
        customerId = -1;
    }

    int menuItemId, quantity;
    printf("Enter menu item ID to order: ");
    scanf("%d", &menuItemId);
    printf("Enter quantity: ");
    scanf("%d", &quantity);

    for (int i = 0; i < menuCount; i++) {
        if (menu[i].id == menuItemId && menu[i].quantity >= quantity) {
            menu[i].quantity -= quantity;
            orders[*orderCount].orderId = *orderCount + 1;
            orders[*orderCount].customerId = customerId;
            orders[*orderCount].menuItemId = menuItemId;
            orders[*orderCount].quantity = quantity;
            (*orderCount)++;
            printf(ANSI_COLOR_GREEN "\nOrder placed successfully.\n" ANSI_COLOR_RESET);
            return;
        }
    }
    printf(ANSI_COLOR_RED "\nItem not found or insufficient quantity.\n" ANSI_COLOR_RESET);
}

void viewOrders(struct order orders[], int count) {
    printf("\nOrders:\n");
    for (int i = 0; i < count; i++) {
        printf("Order ID: %d, Customer ID: %d, Menu Item ID: %d, Quantity: %d\n", orders[i].orderId, orders[i].customerId, orders[i].menuItemId, orders[i].quantity);
    }
}

void deleteOrder(struct order orders[], int *count) {
    int orderId;
    printf("\nEnter Order ID to delete: ");
    scanf("%d", &orderId);

    for (int i = 0; i < *count; i++) {
        if (orders[i].orderId == orderId) {
            for (int j = i; j < *count - 1; j++) {
                orders[j] = orders[j + 1];
            }
            (*count)--;
            printf(ANSI_COLOR_GREEN "\nOrder deleted successfully.\n" ANSI_COLOR_RESET);
            return;
        }
    }
    printf(ANSI_COLOR_RED "\nOrder not found.\n" ANSI_COLOR_RESET);
}


void addReservation(struct reservation reservations[], int *reservationCount, struct customer customers[], int customerCount) {
    if (*reservationCount >= MAX_RESERVATIONS) {
        printf(ANSI_COLOR_RED "\nReservation limit reached.\n" ANSI_COLOR_RESET);
        return;
    }

    int customerId;
    printf("\nEnter your customer ID: ");
    scanf("%d", &customerId);

    int customerFound = 0;
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].id == customerId) {
            customerFound = 1;
            break;
        }
    }
    if (!customerFound) {
        printf(ANSI_COLOR_RED "\nCustomer not found.\n" ANSI_COLOR_RESET);
        return;
    }

    reservations[*reservationCount].reservationId = *reservationCount + 1;
    reservations[*reservationCount].customerId = customerId;
    printf("Enter date (YYYY-MM-DD): ");
    scanf("%s", reservations[*reservationCount].date);
    printf("Enter time (HH:MM): ");
    scanf("%s", reservations[*reservationCount].time);
    printf("Enter number of people: ");
    scanf("%d", &reservations[*reservationCount].numberOfPeople);
    (*reservationCount)++;
    printf(ANSI_COLOR_GREEN "\nReservation added successfully.\n" ANSI_COLOR_RESET);
}

void viewReservations(struct reservation reservations[], int count) {
    printf("\nReservations:\n");
    for (int i = 0; i < count; i++) {
        printf("Reservation ID: %d, Customer ID: %d, Date: %s, Time: %s, Number of People: %d\n", reservations[i].reservationId, reservations[i].customerId, reservations[i].date, reservations[i].time, reservations[i].numberOfPeople);
    }
}


void deleteReservation(struct reservation reservations[], int *count) {
    int reservationId;
    printf("\nEnter Reservation ID to delete: ");
    scanf("%d", &reservationId);

    for (int i = 0; i < *count; i++) {
        if (reservations[i].reservationId == reservationId) {
            for (int j = i; j < *count - 1; j++) {
                reservations[j] = reservations[j + 1];
            }
            (*count)--;
            printf(ANSI_COLOR_GREEN "\nReservation deleted successfully.\n" ANSI_COLOR_RESET);
            return;
        }
    }
    printf(ANSI_COLOR_RED "\nReservation not found.\n" ANSI_COLOR_RESET);
}


void addCustomer(struct customer customers[], int *count) {
    if (*count >= MAX_CUSTOMERS) {
        printf(ANSI_COLOR_RED "\nCustomer limit reached.\n" ANSI_COLOR_RESET);
        return;
    }
    customers[*count].id = *count + 1;
    printf("\nEnter name: ");
    scanf("%s", customers[*count].name);
    printf("Enter contact: ");
    scanf("%s", customers[*count].contact);
    printf("Enter username: ");
    scanf("%s", customers[*count].username);
    printf("Enter password: ");
    scanf("%s", customers[*count].password);
    (*count)++;
    printf(ANSI_COLOR_GREEN "\nCustomer added successfully.\n" ANSI_COLOR_RESET);
}


int customerLogin(struct customer customers[], int count) {
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    printf("\nEnter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    for (int i = 0; i < count; i++) {
        if (strcmp(customers[i].username, username) == 0 && strcmp(customers[i].password, password) == 0) {
            printf(ANSI_COLOR_GREEN "\nLogin successful.\n" ANSI_COLOR_RESET);
            return customers[i].id;
        }
    }
    printf(ANSI_COLOR_RED "\nInvalid username or password.\n" ANSI_COLOR_RESET);
    return -1;
}


void customerSignup(struct customer customers[], int *count) {
    addCustomer(customers, count);
}
