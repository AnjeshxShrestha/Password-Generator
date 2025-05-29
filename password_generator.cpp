#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <stdexcept>

class PasswordGenerator {
private:
    // Character sets
    const std::string LOWERCASE_CHARS = "abcdefghijklmnopqrstuvwxyz";
    const std::string UPPERCASE_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string NUMBER_CHARS = "0123456789";
    const std::string SPECIAL_CHARS = "!@#$%^&*()_+-=[]{}|;:,.<>?";
    
    // Random number generator
    std::random_device rd;
    std::mt19937 generator;
    
public:
    // Constructor initializes the random number generator
    PasswordGenerator() : generator(rd()) {}
    
    // Generate a password with specified options
    std::string generate(
        int length,
        bool includeLowercase = true,
        bool includeUppercase = true,
        bool includeNumbers = true,
        bool includeSpecial = true
    ) {
        // Validate inputs
        if (length <= 0) {
            throw std::invalid_argument("Password length must be positive");
        }
        
        if (!(includeLowercase || includeUppercase || includeNumbers || includeSpecial)) {
            throw std::invalid_argument("At least one character type must be selected");
        }
        
        // Create the character pool based on selected options
        std::string charPool;
        if (includeLowercase) charPool += LOWERCASE_CHARS;
        if (includeUppercase) charPool += UPPERCASE_CHARS;
        if (includeNumbers) charPool += NUMBER_CHARS;
        if (includeSpecial) charPool += SPECIAL_CHARS;
        
        // Create distribution for the random selection
        std::uniform_int_distribution<int> distribution(0, charPool.size() - 1);
        
        // Generate the password
        std::string password;
        for (int i = 0; i < length; ++i) {
            password += charPool[distribution(generator)];
        }
        
        // Ensure at least one character from each selected type is included
        ensureCharacterTypes(password, includeLowercase, includeUppercase, includeNumbers, includeSpecial);
        
        return password;
    }

private:
    // Ensure the password contains at least one of each selected character type
    void ensureCharacterTypes(
        std::string& password,
        bool includeLowercase,
        bool includeUppercase,
        bool includeNumbers,
        bool includeSpecial
    ) {
        // If the password is too short to include all required types, we can't guarantee all types
        int requiredTypes = includeLowercase + includeUppercase + includeNumbers + includeSpecial;
        if (password.length() < requiredTypes) {
            return;
        }
        
        // Check which types are missing
        bool hasLower = !includeLowercase || std::any_of(password.begin(), password.end(), 
                        [this](char c) { return LOWERCASE_CHARS.find(c) != std::string::npos; });
                        
        bool hasUpper = !includeUppercase || std::any_of(password.begin(), password.end(), 
                        [this](char c) { return UPPERCASE_CHARS.find(c) != std::string::npos; });
                        
        bool hasNumber = !includeNumbers || std::any_of(password.begin(), password.end(), 
                        [this](char c) { return NUMBER_CHARS.find(c) != std::string::npos; });
                        
        bool hasSpecial = !includeSpecial || std::any_of(password.begin(), password.end(), 
                        [this](char c) { return SPECIAL_CHARS.find(c) != std::string::npos; });
        
        // Replace characters if needed
        std::uniform_int_distribution<int> indexDist(0, password.length() - 1);
        
        if (!hasLower) {
            std::uniform_int_distribution<int> charDist(0, LOWERCASE_CHARS.size() - 1);
            password[indexDist(generator)] = LOWERCASE_CHARS[charDist(generator)];
        }
        
        if (!hasUpper) {
            std::uniform_int_distribution<int> charDist(0, UPPERCASE_CHARS.size() - 1);
            password[indexDist(generator)] = UPPERCASE_CHARS[charDist(generator)];
        }
        
        if (!hasNumber) {
            std::uniform_int_distribution<int> charDist(0, NUMBER_CHARS.size() - 1);
            password[indexDist(generator)] = NUMBER_CHARS[charDist(generator)];
        }
        
        if (!hasSpecial) {
            std::uniform_int_distribution<int> charDist(0, SPECIAL_CHARS.size() - 1);
            password[indexDist(generator)] = SPECIAL_CHARS[charDist(generator)];
        }
    }
};

// Function to get a validated integer input
int getValidatedIntInput(const std::string& prompt, int minValue = 1, int maxValue = 100) {
    int value;
    bool validInput = false;
    
    do {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        
        try {
            value = std::stoi(input);
            if (value >= minValue && value <= maxValue) {
                validInput = true;
            } else {
                std::cout << "Please enter a value between " << minValue << " and " 
                          << maxValue << ".\n";
            }
        } catch (const std::exception&) {
            std::cout << "Invalid input. Please enter a number.\n";
        }
    } while (!validInput);
    
    return value;
}

// Function to get a validated yes/no input
bool getYesNoInput(const std::string& prompt, bool defaultValue = true) {
    std::string defaultStr = defaultValue ? "Y/n" : "y/N";
    std::string fullPrompt = prompt + " [" + defaultStr + "]: ";
    
    std::string input;
    std::cout << fullPrompt;
    std::getline(std::cin, input);
    
    // If input is empty, return the default
    if (input.empty()) {
        return defaultValue;
    }
    
    // Convert to lowercase for comparison
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    
    // Check for yes responses
    if (input == "y" || input == "yes") {
        return true;
    }
    
    // Check for no responses
    if (input == "n" || input == "no") {
        return false;
    }
    
    // If input is not recognized, return the default
    std::cout << "Invalid input. Using default.\n";
    return defaultValue;
}

int main() {
    std::cout << "Password Generator\n";
    std::cout << "==================\n\n";
    
    PasswordGenerator generator;
    
    try {
        // Get user preferences
        int length = getValidatedIntInput("Enter password length (1-100): ", 1, 100);
        
        bool includeLowercase = getYesNoInput("Include lowercase letters?", true);
        bool includeUppercase = getYesNoInput("Include uppercase letters?", true);
        bool includeNumbers = getYesNoInput("Include numbers?", true);
        bool includeSpecial = getYesNoInput("Include special characters?", true);
        
        // Generate the password
        std::string password = generator.generate(
            length, 
            includeLowercase, 
            includeUppercase, 
            includeNumbers, 
            includeSpecial
        );
        
        // Display the generated password
        std::cout << "\nGenerated Password: " << password << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
