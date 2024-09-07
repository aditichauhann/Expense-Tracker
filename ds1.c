using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

class Program
{
    static double currentIncome = 0;
    static double currentExpense = 0;

    [Serializable]
    class Record
    {
        public double Income { get; set; }
        public double Expense { get; set; }
    }

    [Serializable]
    class Node
    {
        public string Date { get; set; }
        public double Amount { get; set; }
        public string Category { get; set; }
        public Node Next { get; set; }
    }

    static void Main()
    {
        List<Node> income = new List<Node>();
        List<Node> expense = new List<Node>();

        Record record = LoadRecord("Record.bin");
        if (record != null)
        {
            currentIncome = record.Income;
            currentExpense = record.Expense;
        }

        income = LoadData("myincome.bin");
        expense = LoadData("myexpense.bin");

        int option;
        do
        {
            Console.WriteLine("_______________________________________________");
            Console.WriteLine($"|     YOUR INCOME   =      {currentIncome:F2} INR      ");
            Console.WriteLine($"|     YOUR EXPENSE  =      {currentExpense:F2} INR     ");
            Console.WriteLine($"|     YOUR BALANCE  =      {currentIncome - currentExpense:F2} INR     ");
            Console.WriteLine("|______________________________________________|");
            Console.WriteLine("ENTER THE OPTION FROM THE BELOW");
            Console.WriteLine("1.INSERT INCOME");
            Console.WriteLine("2.INSERT EXPENSE");
            Console.WriteLine("3.VIEW INCOME RECORD");
            Console.WriteLine("4.VIEW EXPENSE RECORD");
            Console.WriteLine("5.EXIT");

            if (!int.TryParse(Console.ReadLine(), out option))
            {
                Console.WriteLine("Invalid input. Please enter a number.");
                continue;
            }

            switch (option)
            {
                case 1:
                    Console.WriteLine("**************   ADD INCOME   *****************");
                    Console.WriteLine("Enter The Date(e.g day month year)");
                    string date = Console.ReadLine();

                    Console.WriteLine("Enter The Amount");
                    if (!double.TryParse(Console.ReadLine(), out double amount))
                    {
                        Console.WriteLine("Invalid amount.");
                        continue;
                    }

                    Console.WriteLine("Enter the Category");
                    string category = Console.ReadLine();

                    currentIncome += amount;
                    AddNode(income, date, amount, category);
                    SaveData(income, "myincome.bin");
                    break;

                case 2:
                    Console.WriteLine("**************    ADD EXPENSE   *****************");
                    Console.WriteLine("Enter The Date(e.g day month year)");
                    date = Console.ReadLine();

                    Console.WriteLine("Enter The Amount");
                    if (!double.TryParse(Console.ReadLine(), out amount))
                    {
                        Console.WriteLine("Invalid amount.");
                        continue;
                    }

                    Console.WriteLine("Enter the Category");
                    category = Console.ReadLine();

                    currentExpense += amount;
                    AddNode(expense, date, amount, category);
                    SaveData(expense, "myexpense.bin");
                    break;

                case 3:
                    Console.WriteLine("*********   YOUR INCOME RECORD IS   *******");
                    DisplayData(income);
                    break;

                case 4:
                    Console.WriteLine("*********   YOUR EXPENSE RECORD IS   *******");
                    DisplayData(expense);
                    break;

                case 5:
                    SaveRecord("Record.bin");
                    Console.WriteLine("Exiting...");
                    break;

                default:
                    Console.WriteLine("WRONG OPTION SELECTED - Enter Valid Option");
                    break;
            }
        } while (option != 5);
    }

    static void AddNode(List<Node> list, string date, double amount, string category)
    {
        list.Add(new Node { Date = date, Amount = amount, Category = category });
    }

    static void DisplayData(List<Node> list)
    {
        if (list.Count == 0)
        {
            Console.WriteLine("NO RECORDS AVAILABLE");
        }
        else
        {
            foreach (var node in list)
            {
                Console.WriteLine($"Date: {node.Date}");
                Console.WriteLine($"Amount: {node.Amount:F2} INR");
                Console.WriteLine($"Category: {node.Category}");
                Console.WriteLine();
            }
        }
    }

    static void SaveData(List<Node> list, string fileName)
    {
        using (FileStream fs = new FileStream(fileName, FileMode.Create))
        {
            BinaryFormatter bf = new BinaryFormatter();
            bf.Serialize(fs, list);
        }
        Console.WriteLine($"DATA SAVED SUCCESSFULLY TO {fileName}");
    }

    static List<Node> LoadData(string fileName)
    {
        if (File.Exists(fileName))
        {
            using (FileStream fs = new FileStream(fileName, FileMode.Open))
            {
                BinaryFormatter bf = new BinaryFormatter();
                return (List<Node>)bf.Deserialize(fs);
            }
        }
        return new List<Node>();
    }

    static void SaveRecord(string fileName)
    {
        using (FileStream fs = new FileStream(fileName, FileMode.Create))
        {
            BinaryFormatter bf = new BinaryFormatter();
            bf.Serialize(fs, new Record { Income = currentIncome, Expense = currentExpense });
        }
        Console.WriteLine("RECORD SAVED SUCCESSFULLY");
    }

    static Record LoadRecord(string fileName)
    {
