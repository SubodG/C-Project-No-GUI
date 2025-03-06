#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct product {
    char name[100];
    int price;
    int id;
};


void printMainMenu();
void printAdminMenu();
void printCustomerMenu();
void viewProducts();
void addProduct();
void removeProduct();
void updateProduct();
void checkout();
void viewPurchaseHistory();
void addToCart();
void viewCart();
void savePurchaseHistory();
void clearScreen();
void waitForKeyPress();
void loginAdmin();
void loginCustomer();


void clearScreen() {
    printf("\033[H\033[J"); 
}


void waitForKeyPress() {
    printf("Press any key to return to the previous menu...");
    getchar();
    getchar();
    clearScreen();
}


void printMainMenu() {
    clearScreen();
    printf("\n=================================\n");
    printf("       WELCOME TO THE MART       \n");
    printf("=================================\n");
    printf(" 1. Login as Admin\n");
    printf(" 2. Login as Customer\n");
    printf(" 3. Exit\n");
    printf("=================================\n");
    printf(" Choose an option: ");
}


void printAdminMenu() {
    clearScreen();
    printf("\n==============================\n");
    printf("           ADMIN MENU         \n");
    printf("==============================\n");
    printf(" 1. View Products\n");
    printf(" 2. Add Product\n");
    printf(" 3. Remove Product\n");
    printf(" 4. Update Product\n");
    printf(" 5. View Purchase History\n");
    printf(" 6. Logout\n");
    printf("==============================\n");
    printf(" Choose an option: ");
}


void printCustomerMenu() {
    clearScreen();
    printf("\n==============================\n");
    printf("         CUSTOMER MENU        \n");
    printf("==============================\n");
    printf(" 1. View Products\n");
    printf(" 2. Add to Cart\n");
    printf(" 3. View Cart\n");
    printf(" 4. Checkout\n");
    printf(" 5. Logout\n");
    printf("==============================\n");
    printf(" Choose an option: ");
}


void viewProducts() {
    struct product product;
    FILE *fptr = fopen("products.txt", "r");
    if (fptr == NULL) {
        printf("No products available.\n");
    } else {
        clearScreen();
        printf("\n=================================\n");
        printf("          PRODUCT LIST           \n");
        printf("=================================\n");
        while (fread(&product, sizeof(product), 1, fptr)) {
            printf(" ID: %d | Name: %-15s | Price: %d\n", product.id, product.name, product.price);
        }
        printf("=================================\n");
        fclose(fptr);
    }
    waitForKeyPress();
}


void addProduct() {
    struct product product;
    printf("Enter product ID: ");
    scanf("%d", &product.id);
    
    
    getchar();

    printf("Enter product name: ");
    fgets(product.name, sizeof(product.name), stdin);
    
    
    product.name[strcspn(product.name, "\n")] = '\0';
    
    printf("Enter product price: ");
    scanf("%d", &product.price);
    
    FILE *fptr = fopen("products.txt", "a");
    if (fptr == NULL) {
        printf("Error opening product file.\n");
        return;
    }
    fwrite(&product, sizeof(product), 1, fptr);
    fclose(fptr);
    printf("Product added successfully!\n");
    waitForKeyPress();
}


void removeProduct() {
    int productId;
    printf("Enter the product ID to remove: ");
    scanf("%d", &productId);

    FILE *fptr = fopen("products.txt", "r+");
    if (fptr == NULL) {
        printf("Error opening product file.\n");
        return;
    }

    struct product product;
    FILE *tempFptr = fopen("temp.txt", "w");
    if (tempFptr == NULL) {
        printf("Error opening temporary file.\n");
        fclose(fptr);
        return;
    }

    int found = 0;
    while (fread(&product, sizeof(product), 1, fptr)) {
        if (product.id == productId) {
            found = 1;
            continue; 
        }
        fwrite(&product, sizeof(product), 1, tempFptr);
    }

    fclose(fptr);
    fclose(tempFptr);

    if (found) {
        remove("products.txt");
        rename("temp.txt", "products.txt");
        printf("Product removed successfully!\n");
    } else {
        printf("Product not found.\n");
        remove("temp.txt");
    }
    waitForKeyPress();
}


void updateProduct() {
    int productId;
    printf("Enter the product ID to update: ");
    scanf("%d", &productId);

    struct product product;
    FILE *fptr = fopen("products.txt", "r+");
    if (fptr == NULL) {
        printf("Error opening product file.\n");
        return;
    }

    FILE *tempFptr = fopen("temp.txt", "w");
    if (tempFptr == NULL) {
        printf("Error opening temporary file.\n");
        fclose(fptr);
        return;
    }

    int found = 0;
    while (fread(&product, sizeof(product), 1, fptr)) {
        if (product.id == productId) {
            found = 1;
            printf("Product found: %s | Price: %d\n", product.name, product.price);
            printf("Enter new product name: ");
            getchar();  
            fgets(product.name, sizeof(product.name), stdin);
            product.name[strcspn(product.name, "\n")] = '\0';
            printf("Enter new product price: ");
            scanf("%d", &product.price);
        }
        fwrite(&product, sizeof(product), 1, tempFptr);
    }

    fclose(fptr);
    fclose(tempFptr);

    if (found) {
        remove("products.txt");
        rename("temp.txt", "products.txt");
        printf("Product updated successfully!\n");
    } else {
        printf("Product not found.\n");
        remove("temp.txt");
    }
    waitForKeyPress();
}


void viewPurchaseHistory() {
    FILE *fptr = fopen("buyhistory.txt", "r");
    if (fptr == NULL) {
        printf("No purchase history available.\n");
    } else {
        clearScreen();
        char line[200];
        printf("\n=================================\n");
        printf("        PURCHASE HISTORY         \n");
        printf("=================================\n");
        while (fgets(line, sizeof(line), fptr)) {
            printf("%s", line);
        }
        printf("=================================\n");
        fclose(fptr);
    }
    waitForKeyPress();
}


void savePurchaseHistory() {
    FILE *cartFile = fopen("cart.txt", "r");
    FILE *historyFile = fopen("buyhistory.txt", "a");
    if (cartFile == NULL || historyFile == NULL) {
        printf("Error saving purchase history.\n");
        return;
    }

    char customerName[100], phoneNumber[20];
    printf("Enter customer name: ");
    getchar();  
    fgets(customerName, sizeof(customerName), stdin);
    customerName[strcspn(customerName, "\n")] = '\0';

    printf("Enter customer phone number: ");
    fgets(phoneNumber, sizeof(phoneNumber), stdin);
    phoneNumber[strcspn(phoneNumber, "\n")] = '\0';

    fprintf(historyFile, "Customer: %s | Phone: %s\n", customerName, phoneNumber);

    struct product product;
    while (fread(&product, sizeof(product), 1, cartFile)) {
        fprintf(historyFile, " Product: %s | Price: %d\n", product.name, product.price);
    }

    fprintf(historyFile, "=================================\n");
    fclose(cartFile);
    fclose(historyFile);
}


void addToCart() {
    struct product product;
    int productId;
    printf("Enter product ID to add to cart: ");
    scanf("%d", &productId);

    FILE *fptr = fopen("products.txt", "r");
    if (fptr == NULL) {
        printf("No products available.\n");
        return;
    }

    FILE *cartFptr = fopen("cart.txt", "a");
    int found = 0;
    while (fread(&product, sizeof(product), 1, fptr)) {
        if (product.id == productId) {
            fwrite(&product, sizeof(product), 1, cartFptr);
            printf("Product added to cart: %s\n", product.name);
            found = 1;
            break;
        }
    }

    fclose(fptr);
    fclose(cartFptr);

    if (!found) {
        printf("Product not found.\n");
    }
    waitForKeyPress();
}


void viewCart() {
    struct product product;
    FILE *fptr = fopen("cart.txt", "r");
    if (fptr == NULL) {
        printf("No products in cart.\n");
        waitForKeyPress();
        return;
    }

    clearScreen();
    printf("\n=================================\n");
    printf("           YOUR CART             \n");
    printf("=================================\n");
    while (fread(&product, sizeof(product), 1, fptr)) {
        printf(" Product: %-15s | Price: %d\n", product.name, product.price);
    }
    printf("=================================\n");
    fclose(fptr);
    waitForKeyPress();
}


void checkout() {
    struct product product;
    FILE *fptr = fopen("cart.txt", "r");
    if (fptr == NULL) {
        printf("No products in cart.\n");
        waitForKeyPress();
        return;
    }

    clearScreen();
    printf("\n=================================\n");
    printf("        CHECKOUT SUMMARY         \n");
    printf("=================================\n");
    while (fread(&product, sizeof(product), 1, fptr)) {
        printf(" Purchased: %-15s | Price: %d\n", product.name, product.price);
    }
    printf("=================================\n");
    fclose(fptr);

    
    savePurchaseHistory();
    
    
    remove("cart.txt");
    printf("Checkout complete. Thank you!\n");
    waitForKeyPress();
}


void loginAdmin() {
    char username[50], password[50];
    printf("Enter admin username: ");
    getchar();  
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("Enter admin password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';
    
    if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
        int choice;
        do {
            printAdminMenu();
            scanf("%d", &choice);
            switch (choice) {
                case 1: viewProducts(); break;
                case 2: addProduct(); break;
                case 3: removeProduct(); break;
                case 4: updateProduct(); break;
                case 5: viewPurchaseHistory(); break;
                case 6: printf("Logging out...\n"); break;
                default: printf("Invalid choice.\n");
            }
        } while (choice != 6);
    } else {
        printf("Invalid credentials. Try again.\n");
        waitForKeyPress();
    }
}


void loginCustomer() {
    int choice;
    do {
        printCustomerMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1: viewProducts(); break;
            case 2: addToCart(); break;
            case 3: viewCart(); break;
            case 4: checkout(); break;
            case 5: printf("Logging out...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 5);
}


int main() {
    int choice;
    do {
        printMainMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1: loginAdmin(); break;
            case 2: loginCustomer(); break;
            case 3: printf("Exiting program...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 3);
    return 0;
}
