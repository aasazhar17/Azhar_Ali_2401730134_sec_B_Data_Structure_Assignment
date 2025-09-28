#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
using namespace std;

// =============================================
// 1. WEATHER RECORD ADT (Abstract Data Type)
// =============================================

class WeatherRecord {
private:
    string date;        // Format: DD/MM/YYYY
    string city;        // City name
    double temperature; // Temperature value

public:
    // Constructor - naya record banane ke liye
    WeatherRecord(string d = "", string c = "", double t = 0.0) {
        date = d;
        city = c;
        temperature = t;
    }

    // Getter methods - private data access karne ke liye
    string getDate() const { return date; }
    string getCity() const { return city; }
    double getTemperature() const { return temperature; }

    // Setter methods - values update karne ke liye
    void setDate(string d) { date = d; }
    void setCity(string c) { city = c; }
    void setTemperature(double t) { temperature = t; }

    // Display record details
    void display() const {
        cout << "Date: " << date << ", City: " << city 
             << ", Temperature: " << temperature << "°C" << endl;
    }
};

// =============================================
// 2. MAIN WEATHER DATA STORAGE CLASS
// =============================================

class WeatherDataStorage {
private:
    // 2D array for year-wise, city-wise temperature data
    vector<vector<double>> temperatureMatrix;
    
    // Metadata for rows and columns
    vector<int> years;          // Rows represent years
    vector<string> cities;      // Columns represent cities
    
    // Fast lookup ke liye maps
    map<int, int> yearToIndex;   // Year -> row index mapping
    map<string, int> cityToIndex; // City -> column index mapping
    
    // Sparse data handling ke liye
    double missingValue;        // Sentinel value for missing data
    int totalRecords;
    int presentRecords;

public:
    // Constructor - storage initialize karta hai
    WeatherDataStorage(vector<int> yearList, vector<string> cityList, double missingVal = -999.9) {
        years = yearList;
        cities = cityList;
        missingValue = missingVal;
        totalRecords = 0;
        presentRecords = 0;
        
        // Year aur city to index mapping create karna
        for(int i = 0; i < years.size(); i++) {
            yearToIndex[years[i]] = i;
        }
        for(int j = 0; j < cities.size(); j++) {
            cityToIndex[cities[j]] = j;
        }
        
        // 2D matrix initialize karna with missing values
        int rows = years.size();
        int cols = cities.size();
        temperatureMatrix.resize(rows, vector<double>(cols, missingValue));
        
        cout << "Weather Data Storage initialized with " << rows 
             << " years and " << cols << " cities." << endl;
    }

    // =============================================
    // ADT METHODS: insert, delete, retrieve
    // =============================================

    // Insert a new weather record into the system
    void insert(const WeatherRecord& record) {
        string date = record.getDate();
        string city = record.getCity();
        double temp = record.getTemperature();
        
        // Extract year from date (last 4 characters)
        int year = stoi(date.substr(6, 4));
        
        // Check if year and city exist in our storage
        if(yearToIndex.find(year) == yearToIndex.end()) {
            cout << "Error: Year " << year << " not found in storage!" << endl;
            return;
        }
        if(cityToIndex.find(city) == cityToIndex.end()) {
            cout << "Error: City " << city << " not found in storage!" << endl;
            return;
        }
        
        // Get row and column indices
        int row = yearToIndex[year];
        int col = cityToIndex[city];
        
        // Insert into matrix
        if(temperatureMatrix[row][col] == missingValue) {
            presentRecords++; // New record added
        }
        temperatureMatrix[row][col] = temp;
        totalRecords++;
        
        cout << "INSERTED: ";
        record.display();
    }

    // Remove a weather record based on city and date
    void deleteRecord(string city, string date) {
        // Extract year from date
        int year = stoi(date.substr(6, 4));
        
        // Check if year and city exist
        if(yearToIndex.find(year) == yearToIndex.end() || 
           cityToIndex.find(city) == cityToIndex.end()) {
            cout << "Error: Record not found for deletion!" << endl;
            return;
        }
        
        int row = yearToIndex[year];
        int col = cityToIndex[city];
        
        if(temperatureMatrix[row][col] != missingValue) {
            temperatureMatrix[row][col] = missingValue;
            presentRecords--;
            cout << "DELETED: Record for " << city << " on " << date << endl;
        } else {
            cout << "No record found to delete for " << city << " on " << date << endl;
        }
    }

    // Retrieve temperature data for specific city and year
    void retrieve(string city, int year) {
        if(yearToIndex.find(year) == yearToIndex.end() || 
           cityToIndex.find(city) == cityToIndex.end()) {
            cout << "Error: Data not available for " << city << " in " << year << endl;
            return;
        }
        
        int row = yearToIndex[year];
        int col = cityToIndex[city];
        double temp = temperatureMatrix[row][col];
        
        if(temp != missingValue) {
            cout << "RETRIEVED: " << city << " in " << year 
                 << " = " << temp << "°C" << endl;
        } else {
            cout << "No data available for " << city << " in " << year << endl;
        }
    }

    // =============================================
    // DATA STORAGE METHODS
    // =============================================

    // Populate the 2D array with sample temperature values
    void populateArray() {
        cout << "\n--- POPULATING 2D ARRAY WITH SAMPLE DATA ---" << endl;
        
        // Sample data for demonstration
        // Year 2023 data
        if(yearToIndex.find(2023) != yearToIndex.end()) {
            int row = yearToIndex[2023];
            if(cityToIndex.find("Delhi") != cityToIndex.end()) 
                temperatureMatrix[row][cityToIndex["Delhi"]] = 28.5;
            if(cityToIndex.find("Mumbai") != cityToIndex.end()) 
                temperatureMatrix[row][cityToIndex["Mumbai"]] = 32.1;
            if(cityToIndex.find("Kolkata") != cityToIndex.end()) 
                temperatureMatrix[row][cityToIndex["Kolkata"]] = 25.8;
        }
        
        // Year 2024 data (some missing values for sparse data demonstration)
        if(yearToIndex.find(2024) != yearToIndex.end()) {
            int row = yearToIndex[2024];
            if(cityToIndex.find("Delhi") != cityToIndex.end()) 
                temperatureMatrix[row][cityToIndex["Delhi"]] = 29.2;
            if(cityToIndex.find("Mumbai") != cityToIndex.end()) 
                temperatureMatrix[row][cityToIndex["Mumbai"]] = 33.5;
            // Kolkata data missing intentionally (sparse data)
        }
        
        // Year 2025 data
        if(yearToIndex.find(2025) != yearToIndex.end()) {
            int row = yearToIndex[2025];
            if(cityToIndex.find("Delhi") != cityToIndex.end()) 
                temperatureMatrix[row][cityToIndex["Delhi"]] = 30.1;
            // Mumbai data missing
            if(cityToIndex.find("Kolkata") != cityToIndex.end()) 
                temperatureMatrix[row][cityToIndex["Kolkata"]] = 26.5;
        }
        
        cout << "2D array populated with sample data successfully!" << endl;
        
        // Update record counts
        updateRecordCounts();
    }

    // Update total and present record counts
    void updateRecordCounts() {
        totalRecords = years.size() * cities.size();
        presentRecords = 0;
        
        for(int i = 0; i < years.size(); i++) {
            for(int j = 0; j < cities.size(); j++) {
                if(temperatureMatrix[i][j] != missingValue) {
                    presentRecords++;
                }
            }
        }
    }

    // =============================================
    // ACCESS METHODS: Row-major and Column-major
    // =============================================

    // Implement row-major representation for accessing data
    void rowMajorAccess() {
        cout << "\n--- ROW-MAJOR ACCESS (Year-wise Data) ---" << endl;
        cout << "Efficiency: Better for accessing all cities data for specific years" << endl;
        cout << "Memory Layout: Contiguous years data stored together\n" << endl;
        
        for(int i = 0; i < years.size(); i++) {
            cout << "Year " << years[i] << ":" << endl;
            for(int j = 0; j < cities.size(); j++) {
                double temp = temperatureMatrix[i][j];
                if(temp != missingValue) {
                    cout << "  " << cities[j] << ": " << temp << "°C" << endl;
                } else {
                    cout << "  " << cities[j] << ": Data missing" << endl;
                }
            }
            cout << endl;
        }
    }

    // Implement column-major representation for accessing data
    void columnMajorAccess() {
        cout << "\n--- COLUMN-MAJOR ACCESS (City-wise Data) ---" << endl;
        cout << "Efficiency: Better for accessing historical data for specific cities" << endl;
        cout << "Memory Layout: Contiguous cities data stored together\n" << endl;
        
        for(int j = 0; j < cities.size(); j++) {
            cout << "City " << cities[j] << ":" << endl;
            for(int i = 0; i < years.size(); i++) {
                double temp = temperatureMatrix[i][j];
                if(temp != missingValue) {
                    cout << "  " << years[i] << ": " << temp << "°C" << endl;
                } else {
                    cout << "  " << years[i] << ": Data missing" << endl;
                }
            }
            cout << endl;
        }
    }

    // Compare row-major vs column-major efficiency
    void compareAccessMethods() {
        cout << "\n--- COMPARISON: Row-Major vs Column-Major ---" << endl;
        cout << "ROW-MAJOR Advantages:" << endl;
        cout << "- Better cache performance for year-wise queries" << endl;
        cout << "- Faster when accessing all cities for one year" << endl;
        cout << "- Memory layout matches temporal data access patterns" << endl;
        
        cout << "\nCOLUMN-MAJOR Advantages:" << endl;
        cout << "- Better for city-wise historical analysis" << endl;
        cout << "- Faster when analyzing one city across multiple years" << endl;
        cout << "- Efficient for column-wise operations" << endl;
        
        cout << "\nConclusion: Choice depends on query patterns." << endl;
        cout << "For weather data analysis, ROW-MAJOR is generally better." << endl;
    }

    // =============================================
    // SPARSE DATA HANDLING
    // =============================================

    // Manage scenarios with missing data using sentinel values
    void handleSparseData() {
        cout << "\n--- SPARSE DATA HANDLING ANALYSIS ---" << endl;
        
        updateRecordCounts();
        int missingRecords = totalRecords - presentRecords;
        double sparsity = (double)missingRecords / totalRecords * 100;
        
        cout << "Total possible records: " << totalRecords << endl;
        cout << "Present records: " << presentRecords << endl;
        cout << "Missing records: " << missingRecords << endl;
        cout << "Sparsity level: " << fixed << setprecision(2) << sparsity << "%" << endl;
        
        cout << "\nCurrent Strategy: Sentinel Values (" << missingValue << ")" << endl;
        cout << "- Simple to implement" << endl;
        cout << "- Easy to check for missing data" << endl;
        cout << "- Works well for moderately sparse data" << endl;
        
        // Recommendation based on sparsity
        if(sparsity > 70) {
            cout << "\nRECOMMENDATION: Use SPARSE MATRIX representation" << endl;
            cout << "- Store only non-missing values" << endl;
            cout << "- Save significant memory for highly sparse data" << endl;
            cout << "- Implementation: Store (row, col, value) triplets" << endl;
        } else if(sparsity > 30) {
            cout << "\nRECOMMENDATION: Current sentinel approach is adequate" << endl;
            cout << "- Balance between simplicity and efficiency" << endl;
        } else {
            cout << "\nRECOMMENDATION: Dense matrix is efficient" << endl;
            cout << "- Low sparsity doesn't justify sparse matrix overhead" << endl;
        }
        
        // Display missing data details
        cout << "\nMissing Data Details:" << endl;
        for(int i = 0; i < years.size(); i++) {
            for(int j = 0; j < cities.size(); j++) {
                if(temperatureMatrix[i][j] == missingValue) {
                    cout << "- " << cities[j] << " in " << years[i] << endl;
                }
            }
        }
    }

    // =============================================
    // COMPLEXITY ANALYSIS
    // =============================================

    // Provide time and space complexity analysis
    void analyzeComplexity() {
        cout << "\n--- TIME AND SPACE COMPLEXITY ANALYSIS ---" << endl;
        
        cout << "\nTIME COMPLEXITY:" << endl;
        cout << "INSERT Operation:" << endl;
        cout << "- Best Case: O(1) - Direct index access with maps" << endl;
        cout << "- Worst Case: O(1) - Constant time with pre-computed indices" << endl;
        
        cout << "\nDELETE Operation:" << endl;
        cout << "- Best Case: O(1) - Direct index access" << endl;
        cout << "- Worst Case: O(1) - Constant time operation" << endl;
        
        cout << "\nRETRIEVE Operation:" << endl;
        cout << "- Best Case: O(1) - Direct index access" << endl;
        cout << "- Worst Case: O(1) - Constant time with map lookups" << endl;
        
        cout << "\nROW-MAJOR Access:" << endl;
        cout << "- Time: O(rows × columns) = O(" << years.size() << " × " << cities.size() << ")" << endl;
        
        cout << "\nCOLUMN-MAJOR Access:" << endl;
        cout << "- Time: O(columns × rows) = O(" << cities.size() << " × " << years.size() << ")" << endl;
        
        cout << "\nSPACE COMPLEXITY:" << endl;
        cout << "- 2D Array: O(rows × columns) = O(" << years.size() << " × " << cities.size() << ")" << endl;
        cout << "- Maps for indexing: O(rows + columns)" << endl;
        cout << "- Total: O(rows × columns) - Dominated by 2D matrix" << endl;
        
        cout << "\nMEMORY USAGE CALCULATION:" << endl;
        int matrixSize = years.size() * cities.size() * sizeof(double);
        int mapSize = (years.size() + cities.size()) * sizeof(int);
        cout << "- 2D Matrix: " << matrixSize << " bytes" << endl;
        cout << "- Index Maps: " << mapSize << " bytes" << endl;
        cout << "- Approx Total: " << (matrixSize + mapSize) << " bytes" << endl;
    }

    // =============================================
    // UTILITY METHODS
    // =============================================

    // Display complete matrix
    void displayMatrix() {
        cout << "\n--- COMPLETE TEMPERATURE MATRIX ---" << endl;
        cout << "Rows: Years, Columns: Cities" << endl;
        cout << "Missing data shown as: " << missingValue << endl << endl;
        
        // Header row with cities
        cout << setw(10) << "Year";
        for(string city : cities) {
            cout << setw(12) << city;
        }
        cout << endl;
        
        // Data rows
        for(int i = 0; i < years.size(); i++) {
            cout << setw(10) << years[i];
            for(int j = 0; j < cities.size(); j++) {
                if(temperatureMatrix[i][j] != missingValue) {
                    cout << setw(12) << temperatureMatrix[i][j];
                } else {
                    cout << setw(12) << "MISSING";
                }
            }
            cout << endl;
        }
    }
};

// =============================================
// MAIN FUNCTION - PROGRAM EXECUTION STARTS HERE
// =============================================

int main() {
    cout << "==========================================" << endl;
    cout << "   WEATHER DATA STORAGE SYSTEM - C++" << endl;
    cout << "   Assignment 1 - Data Structures" << endl;
    cout << "==========================================" << endl << endl;

    // Step 1: Initialize weather data storage
    vector<int> years = {2023, 2024, 2025};
    vector<string> cities = {"Delhi", "Mumbai", "Kolkata", "Chennai"};
    
    WeatherDataStorage weatherDB(years, cities);

    // Step 2: Populate with sample data
    weatherDB.populateArray();

    // Step 3: Demonstrate ADT operations
    cout << "\n*** ADT OPERATIONS DEMONSTRATION ***" << endl;
    
    // Insert new records
    WeatherRecord record1("15/01/2023", "Chennai", 31.5);
    WeatherRecord record2("20/06/2024", "Mumbai", 29.8);
    WeatherRecord record3("10/12/2025", "Delhi", 18.2);
    
    weatherDB.insert(record1);
    weatherDB.insert(record2);
    weatherDB.insert(record3);

    // Retrieve operations
    cout << "\n--- RETRIEVE OPERATIONS ---" << endl;
    weatherDB.retrieve("Delhi", 2023);
    weatherDB.retrieve("Mumbai", 2024);
    weatherDB.retrieve("Kolkata", 2025);

    // Delete operation
    cout << "\n--- DELETE OPERATION ---" << endl;
    weatherDB.deleteRecord("Chennai", "15/01/2023");

    // Step 4: Access methods comparison
    weatherDB.rowMajorAccess();
    weatherDB.columnMajorAccess();
    weatherDB.compareAccessMethods();

    // Step 5: Sparse data handling
    weatherDB.handleSparseData();

    // Step 6: Complexity analysis
    weatherDB.analyzeComplexity();

    // Step 7: Display complete matrix
    weatherDB.displayMatrix();

    cout << "\n==========================================" << endl;
    cout << "   ASSIGNMENT REQUIREMENTS COMPLETED:" << endl;
    cout << "   1. Weather Record ADT ✓" << endl;
    cout << "   2. 2D Array Storage ✓" << endl;
    cout << "   3. Row/Column Major Access ✓" << endl;
    cout << "   4. Sparse Data Handling ✓" << endl;
    cout << "   5. Complexity Analysis ✓" << endl;
    cout << "==========================================" << endl;

    return 0;
}