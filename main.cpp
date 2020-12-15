/* 
 * File:   main.cpp
 * Authors: Sergio Hurtado & Jessica Lee
 * Class: CIS 17A
 * Instructor: Dr. Nguyen
 * Created on January 17, 2019, 9:32 PM
 */

/*
 * This program will serve as an employee sales assistance *tool for a cellular service provider.
 * The program will then calculate a customer's up front *cost as well as their monthly cost based on the plan and *type of deviceChoice chosen.
 */

#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>

using namespace std;

const float TAX_RATE = .0775;                 // 7.75% tax rate multiplier
const float PREPAID_BASICS = 40.0;            // Monthly cost of Basics Prepaid plan
const float PREPAID_STANDARD = 55.00;         // Monthly cost of prepaid standard
const float PREPAID_PREMIUM = 75.0;           // Monthly cost of Premium Prepaid
const float POSTPAID_STANDARD = 70.0;         // Monthly cost of Standard Postpaid plan
const float POSTPAID_PREMIUM = 85.0;          // Monthly cost of Premium Postpaid plan
const int NUM_DEVICES = 6;                    // Total number of devices offered
const int FINANCE_TERM = 24;                  // Months of fincancing term
const float ACTIVATION = 25.0;                // Cost of line activation

// Upper bound for parallel arrays
const int ARR_MAX = 6;
const int ARR_MIN = 1;
// Array containing names of phones offered
string PHONE_CHOICES[6] = {"Iphone XS Max", "Iphone XS", "Iphone XR",
                                    "Samsung Note 9", "Samsung S9+", "Samsung S9"};
// Array containing prices of associated phones
float PHONE_PRICES[6] = {1100.00, 1000.00, 750.00,
                                  1000.00, 840.00, 720.00};

// Function prototypes

/* mainMenu() 
 * This function will display the main menu for the user to
 * select which type of plan he/she would like to choose.
 */
void mainMenu();

/* prepaidMenu()
 * This function will display the prepaid options if customer 
 * selects the prepaid menu under the main menu.
 */
void prepaidMenu();

/* postpaidMenu()
 * This function will display the postpaid options if customer 
 * selects the postpaid menu under the main menu.
 */
void postpaidMenu();

/* getPrepaidCost()
 * This function will calculate the total monthly cost of the customer's
 * prepaid bill
 */
float getPrepaidCost(char);

/* getPrepaidCost()
 * This function will calculate the total monthly cost of the customer's
 * postpaid bill after taxes
 */
void getPostpaidCost(char, int, float, float&, float&, float&, float&);

/* getPrepaidDevice()
 * This function will display the menu of devices available and get the user's choice 
 * from a list. It will then return the value of the index in the for the deviceChoice chosen.
 */
void getPrepaidDevice(char);

/* getCreditTier()
 * This function will take in the customer's credit score and return the pricing Tier
 * for purchasing devices
 */
void getCreditTier(int&, float &);

int main(int argc, char** argv) 
{
  // Declare variables
  int mainChoice;         // User selects 1 for Prepaid or 2 for Postpaid
  int creditTier;         // To store customer's credit Tier
  int creditScore;        // User's credit score
  float percentDown = 0;  // Percent of deviceChoice price due up front based on credit tier
  char subMenuChoice;     // User selects a, b, or c depending on plan choice
  char prepaidDevice;     // To hold y or n if customer is purchasing a 
                          // deviceChoice on prepaid plan
  char tryAgain = 'N';
  // Loop rerun program at the end if user desires

  // Variables to be passed as reference to output to file
  float downPmtAmt;       // Dollar amount down payment
  float otdCost;          // Dollar amount up front cost
  float taxAmt;           // Dollar amount to be paid in taxes
  float monthlyCost;      // Total cost per month

  do{

  // Display main menu
  do{                     // Input validation loop
      mainMenu();
      cin >> mainChoice;
    } while (mainChoice != 1 && mainChoice != 2);
    
  // Send user to correct menu depending on mainChoice
  if (mainChoice == 1)
  {
      // Display submenu for Prepaid
      do{                 // Input validation loop
          prepaidMenu();
          cin >> subMenuChoice;
      } while (toupper(subMenuChoice) != 'A' && 
                toupper(subMenuChoice) != 'B' && 
                toupper(subMenuChoice) != 'C');
      do{
          // Find out if the customer will be purchasing a deviceChoice
          cout << "Is the customer interested in purchasing a device today? ('Y'/'N')? ";
          cin >> prepaidDevice;
      } while (prepaidDevice != 'y' && prepaidDevice != 'Y' && prepaidDevice != 'n' && prepaidDevice != 'N');
      // Total for prepaid if no phone is financed
      if (prepaidDevice == 'N' || prepaidDevice =='n')
      {   // Display total up front cost
        cout << fixed << setprecision (2) 
              << "\nYour Total up front cost is: $"
              << getPrepaidCost(subMenuChoice) + ((ACTIVATION) + (ACTIVATION * TAX_RATE));
        // Calculate and display monthly cost for prepaid
        cout << fixed << setprecision (2) 
              << "\nYour Total monthly cost for this plan will be: $" << getPrepaidCost(subMenuChoice); 
      }
      else
      {
        getPrepaidDevice(subMenuChoice);
      }  
  }
    else if (mainChoice == 2)
    {
        // Display submenu for Postpaid
        do{                 // Input validation loop
            postpaidMenu();
            cin >> subMenuChoice;
        } while (toupper(subMenuChoice) != 'A' && 
                 toupper(subMenuChoice) != 'B');
          // Get user's credit pricing Tier
          cout << "\n\nWhat is the customer's credit score? ";
          cin >> creditScore;
          getCreditTier(creditScore, percentDown);
        // Provide correct cost based on user choice
        getPostpaidCost(subMenuChoice, creditTier, percentDown, downPmtAmt, otdCost, taxAmt, monthlyCost);
    }
    else
    {
        cout << "Ooops! It seems as though something went wrong\n"
                "Please try again.";
    }
  // Output to file
  ofstream myfile;     // Create receipt file
  myfile.open ("receipt.txt");
  // Write informationto file
  myfile << "-------- SMART MOBILE RECEIPT --------\n" << endl;
  myfile << "Down Payment:" << fixed << setprecision(2) << setw(18)          << "$" << downPmtAmt << endl;
  myfile << "Taxes:" << setw(25) << "$" << taxAmt << endl;
  myfile << "Total Out the Door:" << setw(12) << "$" << otdCost << endl;
  myfile << "\nTotal Cost Per Month:" << setw(10) << "$" <<  monthlyCost << endl; 
  myfile << "\n*********** THANK YOU! ***********\n";
  myfile.close();  

    // Ask user if they would like to try again
    cout << "\n\n\nWould you like to start a new quote ('Y'/'N')? ";
    cin >> tryAgain;
  } while (tryAgain == 'Y' || tryAgain == 'y');
    
    return 0;
}

// Function Definitions

/* mainMenu() 
 * This function will display the main menu for the user to
 * select which type of plan he/she would like to choose.
 */
void mainMenu()
{
    cout << setw(40) << "WELCOME TO SMART MOBILE!\n\n" << endl;
    cout << setw(52) 
         << "Please select from the following options. (1-2):\n\n";
    cout << setw(19) << "(1) Prepaid" << setw(28) << "(2) Postpaid\n\n";
}

/* prepaidMenu()
 * This function will display the prepaid options if customer 
 * selects the prepaid menu under the main menu.
 */
void prepaidMenu()
{
    cout << setw(56) << "********||PREPAID||********\n\n";
    cout << setw(13) << "(A) BASICS" << setw(34) << "(B) STANDARD" << setw(34)
                     << "(C) PREMIUM\n\n";
    // Columns unlimited talk
    cout << setw(7) << "* Unlimited talk" << setw(32) 
                     << "* Unlimited talk" << setw(36)
                     << "* Unlimited talk\n";
    // Columns unlimited text
    cout << setw(7) << "* Unlimited text" << setw(32) 
                     << "* Unlimited text" << setw(36)
                     << "* Unlimited text\n";
    // Columns regarding data
    cout << setw(3) << "* 4 GB High Speed Data" << setw(33) 
                     << "* 10 GB High Speed Data" << setw(41)
                     << "* Unlimited High Speed Data\n\n";
    // Columns regarding price
    cout << setw(12) << "$40" << setw(32) 
                     << "$55" << setw(36)
                     << "$75\n\n";
    // Get user's selection
    cout << "Please choose (A, B, or C): ";
}

/* postpaidMenu()
 * This function will display the postpaid options if customer 
 * selects the postpaid menu under the main menu.
 */
void postpaidMenu()
{
     cout << setw(44) << "********||POSTPAID||********\n\n";
    cout << setw(15) << "(A) STANDARD" << setw(35)
                     << "(B) PREMIUM\n\n";
    // Columns unlimited talk
    cout << setw(7) << "* Unlimited talk" << setw(36)
                     << "* Unlimited talk\n";
    // Columns unlimited text
    cout << setw(7) << "* Unlimited text" << setw(36)
                     << "* Unlimited text\n";
    // Columns regarding data
    cout << setw(8) << "* Unlimited High Speed Data" << setw(36)
                     << "* Unlimited High Speed Data\n";
    // Additional column for Premium plan hotspot
    cout << setw(62) << "* 25 GB High Speed Hotspot\n";
    // Additional column for Premium plan HD Video
    cout << setw(57) << "* HD Video Streaming\n\n";
    // Columns regarding price
    cout << setw(14) << "$70" << setw(36)
                     << "$85\n\n";
    // Get user's selection
    cout << "Please choose (A or B): ";    
}

/* getPrepaidCost()
 * This function will calculate the total monthly cost of the customer's
 * prepaid bill
 */
float getPrepaidCost(char y)
{
    if (y == 'a' || y == 'A')
    {
        return PREPAID_BASICS + (PREPAID_BASICS * TAX_RATE);
    }
    else if (y == 'b' || y == 'B')
    {
        return PREPAID_STANDARD + (PREPAID_STANDARD * TAX_RATE);
    }
    else if (y == 'c' || y == 'C')
    {
        return PREPAID_PREMIUM + (PREPAID_PREMIUM * TAX_RATE);
    }
    else return 0;
}

/* getPrepaidDevice()
 * This function will display the menu of devices available and get the user's choice 
 * from a list. It will then return the value of the index in the for the deviceChoice chosen.
 */
void getPrepaidDevice(char x)
{
  int choice = toupper(x); // Menu choice that the user chose under prepaid menu
  int deviceChoice;        // Number from menu of deviceChoice chosen
  float taxAmt;            // Dollar amount of taxes paid
  float otdCost;           // Dollar amount to be paid in total out the door
  float planCost;          // Hold the cost of prepaid plan chosen by user

  // Which prepaid service option did the user choose
  if (choice == 'A')
  {planCost = PREPAID_BASICS;}
  else if (choice == 'B')
  {planCost = PREPAID_STANDARD;}
  else 
  {planCost = PREPAID_PREMIUM;}

  // Display devices and their corresponding prices
  do{
    cout << "\n\nPLEASE CHOOSE FROM THE FOLLOWING DEVICES AVAILABLE PURCHASE\n\n";
    // Display the Array of deviceChoice names
    for (int i = 0; i < NUM_DEVICES; i++)
    {
      // Get string length of each element in array for outpur formatting
      string str (PHONE_CHOICES[i]);    
      // Variable to be used as part of setw for outpur formatting                    
      int formatVar = str.length();
      // Display formatted list of devices and prices
      cout << i+1 << ") " << right << PHONE_CHOICES[i] << 
      setw(40 - formatVar) << "$ " << PHONE_PRICES[i] << endl << endl;
    }
    // Get user's deviceChoice choice
    cin >> deviceChoice;
  } while (deviceChoice < ARR_MIN && deviceChoice > ARR_MAX);
  // Choice Iphone XS Max
  if (deviceChoice == 1)
  {
    // Phone variable
    int phone = 0;
    // Calculate amount to be paid in taxes
    taxAmt = (PHONE_PRICES[phone] + ACTIVATION + planCost) * TAX_RATE;
    cout << "Total amount of taxes: $" <<fixed << setprecision(2) << taxAmt << endl;
    // Calculate and display cost if phone is purchased outright
    cout << PHONE_CHOICES[phone] << ": $" << PHONE_PRICES[phone] << endl;
    // Out the door cost if phone is purchased outright
    otdCost = (PHONE_PRICES[phone] + taxAmt + ACTIVATION + planCost);
    cout << "Out the door cost: $" << otdCost << endl;
  }
  // Choice Iphone X
  else if (deviceChoice == 2)
  {
    // Phone variable
    int phone = 1;
    // Calculate amount to be paid in taxes
    taxAmt = (PHONE_PRICES[phone] + ACTIVATION + planCost) * TAX_RATE;
    cout << "Total amount of taxes: $" << fixed << setprecision(2) <<  taxAmt << endl;
    // Calculate and display cost if phone is purchased outright
    cout << PHONE_CHOICES[phone] << ": $" << PHONE_PRICES[phone] << endl;
    // Out the door cost if phone is purchased outright
    otdCost = (PHONE_PRICES[phone] + taxAmt + ACTIVATION + planCost);
    cout << "Out the door cost: $" << otdCost << endl;
  }
  // Choice Iphone XR
  else if (deviceChoice == 3)
  {
    // Phone variable
    int phone = 2;
    // Calculate amount to be paid in taxes
    taxAmt = (PHONE_PRICES[phone] + ACTIVATION + planCost) * TAX_RATE;
    cout << "Total amount of taxes: $" <<fixed << setprecision(2) << taxAmt << endl;
    // Calculate and display cost if phone is purchased outright
    cout << PHONE_CHOICES[phone] << ": $" << PHONE_PRICES[phone] << endl;
    // Out the door cost if phone is purchased outright
    otdCost = (PHONE_PRICES[phone] + taxAmt + ACTIVATION + planCost);
    cout << "Out the door cost: $" << otdCost << endl;
  }
  // Choice Samsung Note 9
  else if (deviceChoice == 4)
  {
    // Phone variable
    int phone = 3;
    // Calculate amount to be paid in taxes
    taxAmt = (PHONE_PRICES[phone] + ACTIVATION + planCost) * TAX_RATE;
    cout << "Total amount of taxes: $" <<fixed << setprecision(2) << taxAmt << endl;
    // Calculate and display cost if phone is purchased outright
    cout << PHONE_CHOICES[phone] << ": $" << PHONE_PRICES[phone] << endl;
    // Out the door cost if phone is purchased outright
    otdCost = (PHONE_PRICES[phone] + taxAmt + ACTIVATION + planCost);
    cout << "Out the door cost: $" << otdCost << endl;
  }      
  // Choice Samsung S9+
  else if (deviceChoice == 5)
  {
    // Phone variable
    int phone = 4;
    // Calculate amount to be paid in taxes
    taxAmt = (PHONE_PRICES[phone] + ACTIVATION + planCost) * TAX_RATE;
    cout << "Total amount of taxes: $" <<fixed << setprecision(2) << taxAmt << endl;
    // Calculate and display cost if phone is purchased outright
    cout << PHONE_CHOICES[phone] << ": $" << PHONE_PRICES[phone] << endl;
    // Out the door cost if phone is purchased outright
    otdCost = (PHONE_PRICES[phone] + taxAmt + ACTIVATION + planCost);
    cout << "Out the door cost: $" << otdCost << endl;
  }    
  // Choice Samsung S9
  else if (deviceChoice == 6)
  {
    // Phone variable
    int phone = 5;
    // Calculate amount to be paid in taxes
    taxAmt = (PHONE_PRICES[phone] + ACTIVATION + planCost) * TAX_RATE;
    cout << "Total amount of taxes: $" << fixed << setprecision(2) << taxAmt << endl;
    // Calculate and display cost if phone is purchased outright
    cout << PHONE_CHOICES[phone] << ": $" << PHONE_PRICES[phone] << endl;
    // Out the door cost if phone is purchased outright
    otdCost = (PHONE_PRICES[phone] + taxAmt + ACTIVATION + planCost);
    cout << "Out the door cost: $" << otdCost << endl;
  }       
  // Default choice
  else
    cout << "You have entered an invalid choice!" << endl;
}

/* getCreditTier()
 * This function will take in the customer's credit score and return the pricing Tier
 * for purchasing devices
 */
void getCreditTier(int &score, float &down)
{
  // Customer's pricing tier to be returned
  int pricingTier;           
  // Customer credit score
  int credScore = score;      
  do{   
    // Break down customer's pricing tier based on credit score
    // Tier 1
    if (credScore <= 800 && credScore >= 750)
    {
      pricingTier = 1;
      down = 0.0;
    }
    // Tier 2
    else if (credScore < 750 && credScore >= 700)
    {
      pricingTier = 2;
      down = .10;
    }
    // Tier 3
    else if (credScore < 700 && credScore >= 650)
    {
      pricingTier = 3;
      down = .20;
    }
    // Tier 4
    else if (credScore < 650 && credScore >= 600)
    {
      pricingTier = 4;
      down = .30;
    }
    // Tier 5
    else if (credScore < 600 && credScore >= 550)
    {
      pricingTier = 5;
      down = .40;
    }
    // Tier 6
    else if (credScore < 550 && credScore >= 500)
    {
      pricingTier = 6;
      down = .50;
    }
    // Tier 7
    else if (credScore < 500 && credScore >= 450)
    {
      pricingTier = 7;
      down = .60;
    }
    // Tier 8
    else if (credScore < 450 && credScore >= 400)
    {
      pricingTier = 8;
      down = .70;
    }
    // Tier 9
    else if (credScore < 400 && credScore >= 350)
    {
      pricingTier = 9;
      down = .80;
    }
    // Tier 10
    else if (credScore < 350 && credScore >= 300)
    {
      pricingTier = 10;
      down = .90;
    }
    else
    {
      cout << "\nThe credit score you entered is invalid!" << endl;
      cout << "Please try again." << endl;
      cout << "\n\nWhat is the customer's credit score? ";
      cin >> credScore;
    }
  } while (credScore < 350 || credScore >850);
}

/* getPostpaidCost()
 * This function will calculate the total monthly cost of the customer's
 * postpaid bill after taxes
 */
void getPostpaidCost(char x, int cred, float down,
                     float &downPmtAmt, float &otdCost,
                     float &taxAmt, float &monthlyCost)
{
  char device;                  // Flag to identify if a deviceChoice is being purchased
  char switchVar = x;           // Character pass in to be switched on
  otdCost = 0;            // To hold total out the door cost
  monthlyCost = 0;        // To hold monthly total cost
  float downPmtPcnt = down;     // Down payment percent for the deviceChoice
  downPmtAmt = 0;         // Dollar amount to be paid as a down payment
  taxAmt = 0;             // Dollar amount to be paid in taxes
  int deviceChoice = 0;         // To hold user's choice of deviceChoice
  int creditTier = cred;        // User's Credit Tier

  // Switch on letter input by user to provide proper prompt for postpaid
  switch (switchVar)
  {
    // Case Standard
    case 'A':
    case 'a':
      // User selected Standard Postpaid plan
      // Is the customer purchasing a deviceChoice as well?
      char financing;
      bool financeFlag;
      do
      {
        cout << "\n\nWill the customer be purchasing a device as well? ('Y'/'N')?" << endl;
        cin >> device;
      } while (toupper(device) != 'Y' && toupper(device) != 'N');

      // Switch on wether the customer is purchasing a deviceChoice
      switch (device)
      {
        case 'Y':
        case 'y':
          // Output prices to 2 decimal places
          cout << fixed << setprecision(2);
          // Is the customer looking to finacne the deviceChoice
          cout << "\nWill the customer be financing this device? ('Y'/'N')?" << endl;
          cin >> financing;
          // Set financing flag
          if (financing == 'y' || financing == 'Y')
          {
            financeFlag = true;
          }
          else financeFlag = false;
          
          // Display devices and their corresponding prices
          cout << "\n\nPLEASE CHOOSE FROM THE FOLLOWING DEVICES AVAILABLE FOR 24 MONTH FINANCING\n\n";
          // Display the Array of deviceChoice names
          for (int i = 0; i < NUM_DEVICES; i++)
          {
            // Get string length of each element in array for outpur formatting
            string str (PHONE_CHOICES[i]);    
            // Variable to be used as part of setw for outpur formatting                    
            int formatVar = str.length();
            // Display formatted list of devices and prices
            cout << i+1 << ") " << right << PHONE_CHOICES[i] << 
            setw(40 - formatVar) << "$ " << PHONE_PRICES[i] << endl << endl;
          }
          // Get user's deviceChoice choice
          cin >> deviceChoice;
          // Choice Iphone XS Max
          if (deviceChoice == 1)
          {
            // Phone variable
            int phone = 0;
            // Calculate amount to be paid in taxes
            taxAmt = (PHONE_PRICES[phone] + ACTIVATION) * TAX_RATE;
            cout << "Total amount of taxes: $" <<fixed << setprecision(2) << taxAmt << endl;
            // Calculate and display cost if customer is financing deviceChoice
            if (financeFlag)
            {
              // Calculate down payment dollar amount
              downPmtAmt = PHONE_PRICES[phone] * downPmtPcnt;
              cout << "Down Payment amount: $" << downPmtAmt << endl;
              otdCost = downPmtAmt + taxAmt + ACTIVATION;
              cout << "Out the door cost: $" << otdCost << endl;
              // Calculate monthly cost
              monthlyCost = ((PHONE_PRICES[phone] - downPmtAmt) / FINANCE_TERM) + (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
            else
            {
              // Calculate and display cost if phone is purchased outright
              cout << PHONE_CHOICES[phone] << ": $" << PHONE_PRICES[phone] << endl;
              // Out the door cost if phone is purchased outright
              otdCost = (PHONE_PRICES[phone] + taxAmt + ACTIVATION);
              cout << "Out the door cost: $" << otdCost << endl;
              // Monthly cost if phone is purchased outright
              monthlyCost = (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
            
          }
          // Choice Iphone X
          else if (deviceChoice == 2)
          {
            // Phone variable
            int phone = 1;
            // Calculate amount to be paid in taxes
            taxAmt = (PHONE_PRICES[phone] + ACTIVATION) * TAX_RATE;
            cout << "Total amount of taxes: $" <<fixed << setprecision(2) << taxAmt << endl;
            // Calculate out the door cost
            if (financeFlag)
            {
              // Calculate down payment dollar amount
              downPmtAmt = PHONE_PRICES[phone] * downPmtPcnt;
              cout << "Down Payment amount: $" << downPmtAmt << endl;
              otdCost = downPmtAmt + taxAmt + ACTIVATION;
              cout << "Out the door cost: $" << otdCost << endl;
              // Calculate monthly cost
              monthlyCost = ((PHONE_PRICES[phone] - downPmtAmt) / FINANCE_TERM) + (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
            else
            {
              // Device cost
              cout << PHONE_CHOICES[phone] << ": $" << PHONE_PRICES[phone] << endl;
              // Out the door cost if phone is purchased outright
              otdCost = (PHONE_PRICES[phone] + taxAmt + ACTIVATION);
              cout << "Out the door cost: $" << otdCost << endl;
              // Monthly cost if phone is purchased outright
              monthlyCost = (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
          }
          // Choice Iphone XR
          else if (deviceChoice == 3)
          {
            // Phone variable
            int phone = 2;
            // Calculate amount to be paid in taxes
            taxAmt = (PHONE_PRICES[phone] + ACTIVATION) * TAX_RATE;
            cout << "Total amount of taxes: $" <<fixed << setprecision(2)  << taxAmt << endl;
            // Calculate out the door cost
            if (financeFlag)
            {
              // Calculate down payment dollar amount
              downPmtAmt = PHONE_PRICES[phone] * downPmtPcnt;
              cout << "Down Payment amount: $" << downPmtAmt << endl;
              otdCost = downPmtAmt + taxAmt + ACTIVATION;
              cout << "Out the door cost: $" << otdCost << endl;
              // Calculate monthly cost
              monthlyCost = ((PHONE_PRICES[phone] - downPmtAmt) / FINANCE_TERM) + (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
            else
            {
              // Device cost
              cout << PHONE_CHOICES[phone] << ": $" << PHONE_PRICES[phone] << endl;
              // Out the door cost if phone is purchased outright
              otdCost = (PHONE_PRICES[phone] + taxAmt + ACTIVATION);
              cout << "Out the door cost: $" << otdCost << endl;
              // Monthly cost if phone is purchased outright
              monthlyCost = (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }           
          }
          // Choice Samsung Note 9
          else if (deviceChoice == 4)
          {
            // Phone variable
            int phone = 3;
            // Calculate amount to be paid in taxes
            taxAmt = (PHONE_PRICES[phone] + ACTIVATION) * TAX_RATE;
            cout << "Total amount of taxes: $" <<fixed << setprecision(2) << taxAmt << endl;
            // Calculate out the door cost
            if (financeFlag)
            {
              // Calculate down payment dollar amount
              downPmtAmt = PHONE_PRICES[phone] * downPmtPcnt;
              cout << "Down Payment amount: $" << downPmtAmt << endl;
              otdCost = downPmtAmt + taxAmt + ACTIVATION;
              cout << "Out the door cost: $" << otdCost << endl;
              // Calculate monthly cost
              monthlyCost = ((PHONE_PRICES[phone] - downPmtAmt) / FINANCE_TERM) + (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
            else
            {
              // Device cost
              cout << PHONE_CHOICES[phone] << ": $" << PHONE_PRICES[phone] << endl;
              // Out the door cost if phone is purchased outright
              otdCost = (PHONE_PRICES[phone] + taxAmt + ACTIVATION);
              cout << "Out the door cost: $" << otdCost << endl;
              // Monthly cost if phone is purchased outright
              monthlyCost = (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
          }
          // Choice Samsung S9+
          else if (deviceChoice == 5)
          {
           // Phone variable
            int phone = 4;
            // Calculate amount to be paid in taxes
            taxAmt = (PHONE_PRICES[phone] + ACTIVATION) * TAX_RATE;
            cout << "Total amount of taxes: $" <<fixed << setprecision(2) << taxAmt << endl;
            // Calculate out the door cost
            if (financeFlag)
            {
              // Calculate down payment dollar amount
              downPmtAmt = PHONE_PRICES[phone] * downPmtPcnt;
              cout << "Down Payment amount: $" << downPmtAmt << endl;
              otdCost = downPmtAmt + taxAmt + ACTIVATION;
              cout << "Out the door cost: $" << otdCost << endl;
              // Calculate monthly cost
              monthlyCost = ((PHONE_PRICES[phone] - downPmtAmt) / FINANCE_TERM) + (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
            else
            {
              // Device cost
              cout << PHONE_CHOICES[phone] << ": $" << PHONE_PRICES[phone] << endl;
              // Out the door cost if phone is purchased outright
              otdCost = (PHONE_PRICES[phone] + taxAmt + ACTIVATION);
              cout << "Out the door cost: $" << otdCost << endl;
              // Monthly cost if phone is purchased outright
              monthlyCost = (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
          }
          // Choice Samsung S9
          else if (deviceChoice == 6)
          {
           // Phone variable
            int phone = 5;
            // Calculate amount to be paid in taxes
            taxAmt = (PHONE_PRICES[phone] + ACTIVATION) * TAX_RATE;
            cout << "Total amount of taxes: $" <<fixed << setprecision(2) << taxAmt << endl;
            // Calculate out the door cost
            if (financeFlag)
            {
              // Calculate down payment dollar amount
              downPmtAmt = PHONE_PRICES[phone] * downPmtPcnt;
              cout << "Down Payment amount: $" << downPmtAmt << endl;
              otdCost = downPmtAmt + taxAmt + ACTIVATION;
              cout << "Out the door cost: $" << otdCost << endl;
              // Calculate monthly cost
              monthlyCost = ((PHONE_PRICES[phone] - downPmtAmt) / FINANCE_TERM) + (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
            else
            {
              // Device cost
              cout << PHONE_CHOICES[phone] << ": $" << PHONE_PRICES[phone] << endl;
              // Out the door cost if phone is purchased outright
              otdCost = (PHONE_PRICES[phone] + taxAmt + ACTIVATION);
              cout << "Out the door cost: $" << otdCost << endl;
              // Monthly cost if phone is purchased outright
              monthlyCost = (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
          }
          // Default choice
          else
            cout << "You have entered an invalid choice!" << endl;
        break;
        // Case if customer does not wish to purchase a deviceChoice, only service.
        case 'N':
        case 'n':
          // Output prices to 2 decimal places
          cout << fixed << setprecision(2);
          // Calculate and output cost if no deviceChoice is to be purchased
          // Cost if no device is purchased
          // Out the door cost if phone is purchased outright
          otdCost = (ACTIVATION * TAX_RATE) + ACTIVATION;
          cout << "Out the door cost: $" << otdCost << endl;
          // Monthly cost if phone is purchased outright
          monthlyCost = (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
          cout << "Total cost per month: $" << monthlyCost;
        break;
              
        default:
          cout << "You entered an invalid value." << endl;
      }
      // Break for case A
      break;
    // Case Premium
    case 'B':
    case 'b':
      // Output prices to 2 decimal places
          cout << fixed << setprecision(2);
      // User selected Premium Postpaid plan
      // User selected Standard Postpaid plan
      // Is the customer purchasing a deviceChoice as well?
      do
      {
        cout << "\n\nWill the customer be purchasing a device as well? ('Y'/'N')?" << endl;
        cin >> device;
      } while (device != 'y' && device != 'Y' && device != 'n' && device != 'N');
      // Switch based on deviceChoice purchase decision
      switch (device)
      {
        case 'Y':
        case 'y':
          // Is the customer looking to finacne the deviceChoice
          cout << "\nWill the customer be financing this device? ('Y'/'N')?" << endl;
          cin >> financing;
          // Set financing flag
          if (financing == 'y' || financing == 'Y')
          {
            financeFlag = true;
          }
          else financeFlag = false;
          // Display devices and their corresponding prices
          cout << "\n\nPLEASE CHOOSE FROM THIS LIST OF AVAILABLE DEVICES\n\n";
          // Display the Array of deviceChoice names
          for (int i = 0; i < NUM_DEVICES; i++)
          {
            // Get string length of each element in array for outpur formatting
            string str (PHONE_CHOICES[i]);    
            // Variable to be used as part of setw for outpur formatting                    
            int formatVar = str.length();
            // Display formatted list of devices and prices
            cout << i+1 << ") " << right << PHONE_CHOICES[i] << 
            setw(40 - formatVar) << "$ " << PHONE_PRICES[i] << endl << endl;
          }
          // Get user's deviceChoice choice
          cin >> deviceChoice;
          // Choice Iphone XS Max
          if (deviceChoice == 1)
          {
            // Phone variable
            int phone = 0;
            // Calculate amount to be paid in taxes
            taxAmt = (PHONE_PRICES[phone] + ACTIVATION) * TAX_RATE;
            cout << "Total amount of taxes: $" << taxAmt << endl;
            // Calculate out the door cost
            if (financeFlag)
            {
              // Calculate down payment dollar amount
              downPmtAmt = PHONE_PRICES[phone] * downPmtPcnt;
              cout << "Down Payment amount: $" << downPmtAmt << endl;
              otdCost = downPmtAmt + taxAmt + ACTIVATION;
              cout << "Out the door cost: $" << otdCost << endl;
              // Calculate monthly cost
              monthlyCost = ((PHONE_PRICES[phone] - downPmtAmt) / FINANCE_TERM) + (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
            else
            {
              // Device cost
              cout << PHONE_CHOICES[phone] << ": $" << PHONE_PRICES[phone] << endl;
              // Out the door cost if phone is purchased outright
              otdCost = (PHONE_PRICES[phone] + taxAmt + ACTIVATION);
              cout << "Out the door cost: $" << otdCost << endl;
              // Monthly cost if phone is purchased outright
              monthlyCost = (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
          }
          // Choice Iphone X
          else if (deviceChoice == 2)
          {
            // Phone variable
            int phone = 1;
            // Calculate amount to be paid in taxes
            taxAmt = (PHONE_PRICES[phone] + ACTIVATION) * TAX_RATE;
            cout << "Total amount of taxes: $" << taxAmt << endl;
            // Calculate out the door cost
            if (financeFlag)
            {
              // Calculate down payment dollar amount
              downPmtAmt = PHONE_PRICES[phone] * downPmtPcnt;
              cout << "Down Payment amount: $" << downPmtAmt << endl;
              otdCost = downPmtAmt + taxAmt + ACTIVATION;
              cout << "Out the door cost: $" << otdCost << endl;
              // Calculate monthly cost
              monthlyCost = ((PHONE_PRICES[phone] - downPmtAmt) / FINANCE_TERM) + (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
            else
            {
              // Device cost
              cout << PHONE_CHOICES[phone] << ": $" << PHONE_PRICES[phone] << endl;
              // Out the door cost if phone is purchased outright
              otdCost = (PHONE_PRICES[phone] + taxAmt + ACTIVATION);
              cout << "Out the door cost: $" << otdCost << endl;
              // Monthly cost if phone is purchased outright
              monthlyCost = (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
          }
          // Choice Iphone XR
          else if (deviceChoice == 3)
          {
            // Phone variable
            int phone = 2;
            // Calculate amount to be paid in taxes
            taxAmt = (PHONE_PRICES[phone] + ACTIVATION) * TAX_RATE;
            cout << "Total amount of taxes: $" << taxAmt << endl;
            // Calculate out the door cost
            if (financeFlag)
            {
              // Calculate down payment dollar amount
              downPmtAmt = PHONE_PRICES[phone] * downPmtPcnt;
              cout << "Down Payment amount: $" << downPmtAmt << endl;
              otdCost = downPmtAmt + taxAmt + ACTIVATION;
              cout << "Out the door cost: $" << otdCost << endl;
              // Calculate monthly cost
              monthlyCost = ((PHONE_PRICES[phone] - downPmtAmt) / FINANCE_TERM) + (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
            else
            {
              // Device cost
              cout << PHONE_CHOICES[phone] << ": $" << PHONE_PRICES[phone] << endl;
              // Out the door cost if phone is purchased outright
              otdCost = (PHONE_PRICES[phone] + taxAmt + ACTIVATION);
              cout << "Out the door cost: $" << otdCost << endl;
              // Monthly cost if phone is purchased outright
              monthlyCost = (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
          }
          // Choice Samsung Note 9
          else if (deviceChoice == 4)
          {
            // Phone variable
            int phone = 3;
            // Calculate amount to be paid in taxes
            taxAmt = (PHONE_PRICES[phone] + ACTIVATION) * TAX_RATE;
            cout << "Total amount of taxes: $" << taxAmt << endl;
            // Calculate out the door cost
            if (financeFlag)
            {
              // Calculate down payment dollar amount
              downPmtAmt = PHONE_PRICES[phone] * downPmtPcnt;
              cout << "Down Payment amount: $" << downPmtAmt << endl;
              otdCost = downPmtAmt + taxAmt + ACTIVATION;
              cout << "Out the door cost: $" << otdCost << endl;
              // Calculate monthly cost
              monthlyCost = ((PHONE_PRICES[phone] - downPmtAmt) / FINANCE_TERM) + (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
            else
            {
              // Device cost
              cout << PHONE_CHOICES[phone] << ": $" << PHONE_PRICES[phone] << endl;
              // Out the door cost if phone is purchased outright
              otdCost = (PHONE_PRICES[phone] + taxAmt + ACTIVATION);
              cout << "Out the door cost: $" << otdCost << endl;
              // Monthly cost if phone is purchased outright
              monthlyCost = (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
          }
          // Choice Samsung S9+
          else if (deviceChoice == 5)
          {
            // Phone variable
            int phone = 4;
            // Calculate amount to be paid in taxes
            taxAmt = (PHONE_PRICES[phone] + ACTIVATION) * TAX_RATE;
            cout << "Total amount of taxes: $" << taxAmt << endl;
            // Calculate out the door cost
            if (financeFlag)
            {
              // Calculate down payment dollar amount
              downPmtAmt = PHONE_PRICES[phone] * downPmtPcnt;
              cout << "Down Payment amount: $" << downPmtAmt << endl;
              otdCost = downPmtAmt + taxAmt + ACTIVATION;
              cout << "Out the door cost: $" << otdCost << endl;
              // Calculate monthly cost
              monthlyCost = ((PHONE_PRICES[phone] - downPmtAmt) / FINANCE_TERM) + (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
            else
            {
              // Device cost
              cout << PHONE_CHOICES[phone] << ": $" << PHONE_PRICES[phone] << endl;
              // Out the door cost if phone is purchased outright
              otdCost = (PHONE_PRICES[phone] + taxAmt + ACTIVATION);
              cout << "Out the door cost: $" << otdCost << endl;
              // Monthly cost if phone is purchased outright
              monthlyCost = (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
          }
          // Choice Samsung S9
          else if (deviceChoice == 6)
          {
            // Phone variable
            int phone = 5;
            // Calculate amount to be paid in taxes
            taxAmt = (PHONE_PRICES[phone] + ACTIVATION) * TAX_RATE;
            cout << "Total amount of taxes: $" << taxAmt << endl;
            // Calculate out the door cost
            if (financeFlag)
            {
              // Calculate down payment dollar amount
              downPmtAmt = PHONE_PRICES[phone] * downPmtPcnt;
              cout << "Down Payment amount: $" << downPmtAmt << endl;
              otdCost = downPmtAmt + taxAmt + ACTIVATION;
              cout << "Out the door cost: $" << otdCost << endl;
              // Calculate monthly cost
              monthlyCost = ((PHONE_PRICES[phone] - downPmtAmt) / FINANCE_TERM) + (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
            else
            {
              // Device cost
              cout << PHONE_CHOICES[phone] << ": $" << PHONE_PRICES[phone] << endl;
              // Out the door cost if phone is purchased outright
              otdCost = (PHONE_PRICES[phone] + taxAmt + ACTIVATION);
              cout << "Out the door cost: $" << otdCost << endl;
              // Monthly cost if phone is purchased outright
              monthlyCost = (POSTPAID_STANDARD + (POSTPAID_STANDARD * TAX_RATE));
              cout << "Total cost per month: $" << monthlyCost;
            }
          }
          // Default choice
          else
            cout << "You have entered an invalid choice!" << endl;
        break;
        // Case if customer not purchasing deviceChoice, only service.
        case 'N':
        case 'n':
          // Calculate and output cost if no deviceChoice is to be purchased
          // Out the door cost if phone is purchased outright
          otdCost = (ACTIVATION * TAX_RATE) + ACTIVATION;
          cout << "Out the door cost: $" << otdCost << endl;
          // Monthly cost if phone is purchased outright
          monthlyCost = (POSTPAID_PREMIUM + (POSTPAID_PREMIUM * TAX_RATE));
          cout << "Total cost per month: $" << monthlyCost;
        break;
              
        default:
          cout << "You entered an invalid value." << endl;
      }
      // Break for case B
      break;

    default: 
      cout << "Invalid Choice!" << endl;
  }
}
