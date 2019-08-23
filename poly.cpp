#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>

using namespace std;

struct nullPolyException : public exception {
	const char * what() const throw() {
		return "Polynomial cannot be NUll, must have at least one non-zero coefficient.";
	}
};

struct ExponentException : public exception {
	const char * what() const throw() {
		return "Exponent Must be a Positive Integer.";
	}
};

struct node {
	double coef;
	int expo;
	node * next;
};


class Polynomial {
	long double Horn[100];
	node * head = nullptr;
	//adds a node to link list in increasing exponent order
	//if there are multiple of the same exponent, adds them together instead of making a new node
	bool addNode(double co, int ex);
	//deletes any nodes with 0 coefficients (made from subtracting polynomials)
	void cleanup();
public:
	long double computed;
	//constructor
	Polynomial() { 
		computed = 0; 
		for (int i = 0; i < 100; i++) {
			Horn[i] = 1;
		}
	};
	//evaluate brute force, i.e., just multiply each time
	long double brute(double x);
	//evaluate horn, i.e., continue from last exponent
	long double horn(double x);
	//combines the last two to compare
	long double notSparseHelper(double x);
	//My algorithm for Sparse Polynomials, Modified Horn's is the name
	long double sparse(int msb, double x);
	//builds linked list
	bool llistBuilder(); //complete
	bool llistBuilder(bool fromFile); //complete
	//let program decide what evaluation to use
	long double decide(double x);
	//use them all
	void testAll(double x);
	//search for monomial
	bool search(double Scoef, int Sexpo); //complete
	//print polynomial
	void showPoly(); //comeplte
	//gets monomial at certain position
	bool retMon(int pos, double retMe[]); //complete
	//deletes linked list
	void clear(); //complete hopefully
	node* getHead() { return head; }
	//gets count of polynomial size
	int getSize();
	//gets coefficient from exponent
	double findCoeff(int exp);
	//overloaded addition
	Polynomial operator +(Polynomial b) {
		Polynomial a;
		int count = 1;
		double data[2];
		data[0] = 0;
		data[1] = 0;
		while (this->retMon(count++, data)) {
			a.addNode(data[0], static_cast<int>(data[1]));
		}
		count = 1;
		data[0] = 0;
		data[1] = 0;
		while (b.retMon(count++, data)) {
			a.addNode(data[0], static_cast<int>(data[1]));
		}
		return a;
	}
	//overloaded subtraction
	Polynomial operator -(Polynomial a) {
		Polynomial b;
		int count = 1;
		double data[2];
		data[0] = 0;
		data[1] = 0;
		while (this->retMon(count++, data)) {
			b.addNode(data[0], static_cast<int>(data[1]));
		}
		count = 1;
		data[0] = 0;
		data[1] = 0;
		while (a.retMon(count++, data)) {
			b.addNode(-1 * data[0], static_cast<int>(data[1]));
		}
		b.cleanup();
		
		return b;
	}
	//overloaded multiplication
	Polynomial operator *(Polynomial a) {
		Polynomial b;
		int size1 = 0, size2 = 0;
		size1 = this->getSize();
		size2 = a.getSize();
		double data1[2];
		double data2[2];
		for (int i = 1; i <= size1; i++) {
			for (int j = 1; j <= size2; j++) {
				this->retMon(i, data1);
				a.retMon(j, data2);
				b.addNode(data1[0] * data2[0], data1[1] + data2[1]);
			}
		}
		b.cleanup();
		return b;
	}
	//did not need to overload = for some reason. It just worked 
};

void Polynomial::clear() {
	if (head == nullptr) return;
	else {
		node*temp = head;
		while (temp->next != nullptr) {
			temp = temp->next;
			delete head;
			head = temp;
		}
		head = nullptr;
		delete temp;
		return;
	}
}

bool Polynomial::llistBuilder() {
	double coef = 0.0;
	int expo = 0;
	char response = ' ';
	bool first = true;
	do {
		try {
			cin >> coef >> expo;
			if (coef == 0 && expo == 0) break;
			if (first && coef == 0 and expo == 0) {
				throw nullPolyException();
			}
			if (expo < 0) {
				throw ExponentException();
			}
			addNode(coef, expo);
		}
		catch (nullPolyException& e) {
			cout << "\nException Caught: " << e.what() << endl;
			cout << "Would you like to [c]ontinue or [q]uit? ";
			cin >> response;
			if (response == 'q') {
				return false;
			}
		}
		catch (ExponentException& e) {
			cout << "\nException Caught: " << e.what() << endl;
			cout << "The previous line was not recorded. Please continue.\n";
		}
	} while (!(coef == 0 && expo == 0));
	return true;
}

//
bool Polynomial::llistBuilder(bool fromFile) {
	ifstream inFile;
	string fileName;
	char shouldContResponse = ' ';
	while (true) {
		cout << "\nInput the name of the file: ";
		cin >> fileName;
		inFile.open(fileName);
		if (!inFile.is_open()) {
			while (true) {
				cout << "\nFile Failed to open.\nRetry? [y] or [n]: ";
				cin.clear();
				cin.ignore(100, '\n');
				cin >> shouldContResponse;
				if (shouldContResponse == 'n') return false;
				else if (shouldContResponse == 'y') break;
				else {
					cout << "\nInvalid character.\n";
					continue;
				}
			}
		}
		else break;
	}
	double coef = 0.0;
	int expo = 0;
	char response = ' ';
	bool first = true;
	do {
		try {
			inFile >> coef >> expo;
			if (coef == 0 && expo == 0) break;
			if (first && coef == 0 and expo == 0) {
				throw nullPolyException();
			}
			if (expo < 0) {
				throw ExponentException();
			}
			addNode(coef, expo);
		}
		catch (nullPolyException& e) {
			cout << "\nException Caught: " << e.what() << endl;
			cout << "Would you like to [c]ontinue or [q]uit? ";
			cin >> response;
			if (response == 'q') {
				return false;
			}
		}
		catch (ExponentException& e) {
			cout << "\nException Caught: " << e.what() << endl;
			cout << "The previous line was not recorded. Please continue.\n";
		}
	} while (!(coef == 0 && expo == 0));
	inFile.close();
	return true;
}

bool Polynomial::addNode(double co, int ex) {
	//if (co == 0) return false;
	node * temp = new node();
	temp->coef = co;
	temp->expo = ex;
	temp->next = nullptr;
	if (head == nullptr) {
		head = temp;
	}
	else if (ex < head->expo) {
		temp->next = head;
		head = temp;
	}
	else if (ex == head->expo) {
		head->coef += co;
		delete temp;
		return true;
	}
	else {
		node* explore = head;
		while (explore->next != nullptr) {
			if (ex == explore->expo) {
				explore->coef += co;
				delete temp;
				return true;
			}
			if (explore->next->expo > ex) {
				temp->next = explore->next;
				break;
			}
			else {
				explore = explore->next;
			}
		}
		if (ex == explore->expo) {
			explore->coef += co;
			delete temp;
			return true;
		}
		explore->next = temp;
	}
	return true;
}

//search polynomial for monomial of matching coefficient (Scoef) and exponent (Sexpo)
//inputs: double coefficient, int exponent, returns true if found, false otherwise
bool Polynomial::search(double Scoef, int Sexpo) {
	//if the polynomial is empty, return false
	if (head == nullptr) return false;
	else {
		node * explore = head;
		//check if head is monomial
		if (explore->coef == Scoef && explore->expo == Sexpo) return true;
		//so long as there are more monomials, move to the next one and check
		while (explore->next != nullptr)
		{
			explore = explore->next;
			if (explore->coef == Scoef && explore->expo == Sexpo) return true;
		}
		//explored all monomials, didn't find it, return false
		return false;
	}
}

double Polynomial::findCoeff(int exp) {
	if (head == nullptr) return 0;
	else {
		node * explore = head;
		if (explore->expo == exp) return explore->coef;
		while (explore->next != nullptr)
		{
			explore = explore->next;
			if (explore->expo == exp) return explore->coef;
		}
		return 0;
	}
}

void Polynomial::showPoly() {
	if (head == nullptr) return;
	else {
		node * explore = head;
		cout << "(" << explore->coef << ")";
		if (explore->expo != 0) cout << "x^" << explore->expo;
		while (explore->next != nullptr)
		{
			cout << " + ";
			explore = explore->next;
			cout << "(" << explore->coef << ")" << "x^" << explore->expo;
		}
	}
	return;
}

bool Polynomial::retMon(int pos, double retMe[]) {
	if (head == nullptr) {
		retMe[0] = 0;
		retMe[1] = 0;
		return false;
	}
	else {
		node * explore = head;
		for (int i = 1; i < pos; i++) {
			if (explore->next == nullptr) {
				retMe[0] = 0;
				retMe[1] = 0;
				return false;
			}
			else explore = explore->next;
		}
		retMe[0] = explore->coef;
		retMe[1] = explore->expo;
		return true;
	}
	return false;
}

void Polynomial::cleanup() {
	//if the polynomial is empty, return false
	if (head == nullptr) return;
	node* temp = head;
	while (head->coef == 0) {
		if (head->next == nullptr) {
			delete head;
			head = nullptr;
			return;
		}
		temp = head->next;
		delete head;
		head = temp;
	}
	node* explore = temp;
	while (temp->next != nullptr) {
		temp = temp->next;
		if (temp->coef == 0) {
			explore->next = temp->next;
			delete temp;
			temp = explore;
		}
		if (temp->next != nullptr) explore = temp;
	}
	if (temp->coef == 0) {
		explore->next = nullptr;
		delete temp;
	}
	return;
}

int Polynomial::getSize() {
	int size = 0;
	if (head == nullptr) return size;
	node* explore = head;
	while (explore != nullptr) {
		size++;
		explore = explore->next;
	}
	return size;
}

long double Polynomial::decide(double x) {
	if (head == nullptr) return 0;
	
	node * temp = head;
	long double retMe = 0;
	while (temp->next != nullptr) {
		temp = temp->next;
	}
	unsigned int log = temp->expo;
	int count = -1; //since binary numbers start at 2^0
	while (log > 0) {
		log >>= 1;
		count++;
	}
	if (getSize() > 1) {
		temp = head;
		int ex1 = head->expo, ex2 = temp->next->expo;
		if (ex2 - ex1 > count + 6) {
			auto start = std::chrono::high_resolution_clock::now(); //chrono code from OlivierLi on StackOverflow
			retMe = sparse(count, x);
			auto elapsed = std::chrono::high_resolution_clock::now() - start;
			long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
			cout << "\nThe time it took to calculate the sparse polynomial was " << microseconds << " microseconds.\n";
			return retMe;
		}
		while (temp->next->next != nullptr) {
			temp = temp->next;
			ex1 = temp->expo;
			ex2 = temp->next->expo;
			if (ex2 - ex1 > count + 6) {
				auto start = std::chrono::high_resolution_clock::now();
				retMe = sparse(count, x);
				auto elapsed = std::chrono::high_resolution_clock::now() - start;
				long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
				cout << "\nThe time it took to calculate the sparse polynomial was " << microseconds << " microseconds.\n";
				return retMe;
			}
		}
		return notSparseHelper(x);
	}
	return brute(x);
}

//TODO fix Sparse when p0 and p1 are multiplied together
long double Polynomial::sparse(int msb, double x) {
	long double store[20];
	long double retMe = 0;
	int count = 0;
	int expo = 0;
	node*temp = head;
	store[0] = x;
	for (int i = 1; i <= msb; i++) {
		store[i] = store[i - 1] * store[i - 1];
	}
	expo = temp->expo;
	if (expo < msb) {
		for (int i = 0; i < expo; i++) {
			Horn[0] = Horn[0] * x;
		}
	}
	else {
		for (int i = 0; i <= msb; i++) {
			if (expo & 0x1)
				Horn[0] *= store[i];
			expo = expo >> 1;
		}
	}
	retMe = (Horn[0]*temp->coef);
	while (temp->next != nullptr) {
		temp = temp->next;
		count++;
		if (temp->expo - expo > msb) {
			expo = temp->expo;
			for (int i = 0; i <= msb; i++) {
				if (expo & 0x1)
					Horn[count] *= store[i];
				expo = expo >> 1;
			}
		}
		else {
			Horn[count] = Horn[count - 1];
			for (int i = 0; i < temp->expo - expo; i++) {
				Horn[count] *= x;
			}
		}
		retMe += (Horn[count]*temp->coef);
		expo = temp->expo;
	}
	for (int i = 0; i < 100; i++) {
		Horn[i] = 1;
	}
	return retMe;
}

long double Polynomial::notSparseHelper(double x) {
	long double retMe = 0;
	auto start = std::chrono::high_resolution_clock::now();
	retMe = brute(x);
	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
	cout << "\nThe time it took to calculate with Brute was " << microseconds << " microseconds.\n";
	start = std::chrono::high_resolution_clock::now();
	retMe = horn(x);
	elapsed = std::chrono::high_resolution_clock::now() - start;
	microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
	cout << "\nThe time it took to calculate with Horn's Method was " << microseconds << " microseconds.\n";
	return retMe;
}

void Polynomial::testAll(double x) {
	long double retMe = 0;
	auto start = std::chrono::high_resolution_clock::now();
	retMe = brute(x);
	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
	cout << "\nThe time it took to calculate with Brute was " << microseconds << " microseconds.\n";
	cout << "The Brute value was: " << retMe << endl;
	start = std::chrono::high_resolution_clock::now();
	retMe = horn(x);
	elapsed = std::chrono::high_resolution_clock::now() - start;
	microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
	cout << "\nThe time it took to calculate with Horn's Method was " << microseconds << " microseconds.\n";
	cout << "The Horn value was: " << retMe << endl;

	start = std::chrono::high_resolution_clock::now();
	node * temp = head;
	while (temp->next != nullptr) {
		temp = temp->next;
	}
	unsigned int log = temp->expo;
	//count = -1?
	int count = -1; //since binary numbers start at 2^0
	while (log > 0) {
		log >>= 1;
		count++;
	}
	retMe = sparse(count, x);
	elapsed = std::chrono::high_resolution_clock::now() - start;
	microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
	cout << "\nThe time it took to calculate with Sparse's Method was " << microseconds << " microseconds.\n";
	cout << "The Sparse value was: " << retMe << endl;
}

long double Polynomial::brute(double x) {
	int count = getSize();
	long double retMe = 0, y = 1;
	node*temp = head;
	for (int i = 0; i < count; i++) {
		for (int j = 0; j < temp->expo; j++) {
			y *= x;
		}
		retMe += y * temp->coef;
		temp = temp->next;
		y = 1;
	}
	return retMe;
}

long double Polynomial::horn(double x) {
	int count = getSize();
	long double retMe = 0, y = 1;
	int prevExpo = 0;
	node*temp = head;
	for (int i = 0; i < count; i++) {
		for (int j = prevExpo; j < temp->expo; j++) {
			y *= x;
		}
		retMe += y * temp->coef;
		prevExpo = temp->expo;
		temp = temp->next;
	}
	return retMe;
}

void menu() {
	system("cls");
	cout << "****************************************" << endl;
	cout << "|      What would you like to do?      |" << endl;
	cout << "|               [H]elp                 |" << endl;
	cout << "|         [M]ake a Polynomial          |" << endl;
	cout << "|       [S]how the Polynomials         |" << endl;
	cout << "|       [E]valuate Polynomials         |" << endl;
	cout << "|        Sea[R]ch for Monomial         |" << endl;
	cout << "|    [F]ind Coefficient of Exponent    |" << endl;
	cout << "|        [A]dd two Polynomials         |" << endl;
	cout << "|         Get the [D]ifference         |" << endl;
	cout << "|      M[U]ltiply two Polynomials      |" << endl;
	cout << "|               [Q]uit                 |" << endl;
	cout << "|                                      |" << endl;
	cout << "****************************************" << endl;
}

void help() {
	char choice = ' ';
	cout << "\nWhich option would you like to know more about?";
	cin >> choice;
	cin.clear();
	cin.ignore(100, '\n');
	if (choice == 'h' || choice == 'H') {
		cout << "\nLearn what a choice does. You are in it!";
	}
	else if (choice == 'm' || choice == 'M') {
		cout << "\nAllows you to add a Polynomial to a list of polynomials.";
	}
	else if (choice == 's' || choice == 'S') {
		cout << "\nDisplays a Polynomial in standard form.";
	}
	else if (choice == 'e' || choice == 'E') {
		cout << "\nEvaluates a Polynomial with a chosen method.";
	}
	else if (choice == 'r' || choice == 'R') {
		cout << "\nReturns true if a monomial exists in a Polynomial.";
	}
	else if (choice == 'f' || choice == 'F') {
		cout << "\nSearch through all Polynomials for the coefficient for chosen exponent.";
	}
	else if (choice == 'a' || choice == 'A') {
		cout << "\nAdds two polynomials to make a larger polynomial.";
	}
	else if (choice == 'd' || choice == 'D') {
		cout << "\nSubtracts the second Polynomial from the first.";
	}
	else if (choice == 'u' || choice == 'U') {
		cout << "\nMultiplies two polynomials together to get a new polynomial.";
	}
	else if (choice == 'q' || choice == 'Q') {
		cout << "\nExit the program.";
	}
	else {
		cout << "Error, unidentified key. Returning to Menu.\n";
	}
	cout << endl;
	system("pause");
}

void main() {
	vector<Polynomial> Polynomials;
	char choice = ' ';
	int polyIn = 0, second = 1;
	double eval = 0;
	long double retMe = 0;
	while (true) {
		menu();
		cin >> choice;
		cin.clear();
		cin.ignore(100, '\n');
		if (choice == 'h' || choice == 'H') {
			help();
		}
		else if (choice == 'm' || choice == 'M') {
			Polynomial temp;
			cout << "\nInput from a [F]ile or [t]erminal or [q]uit? ";
			cin >> choice;
			cin.clear();
			cin.ignore(100, '\n');
			if (choice == 'F' || choice == 'f') {
				if (temp.llistBuilder(true))
					Polynomials.push_back(temp);
				else {
					cout << "\nQuitting to Menu.\n";
				}
			}
			else if (choice == 't' || choice == 'T') {
				cout << "\nInput the polynomial as (double) coefficient (int) exponent, 0 0 marks the end: \n";
				temp.llistBuilder();
				Polynomials.push_back(temp);
			}
			else {
				cout << "\nQuitting to Menu.";
				system("pause");
			}
		}
		else if (choice == 's' || choice == 'S') {
			if (Polynomials.size() == 0) cout << "\nYou must input a polynomial first!\n";
			else {
				for (int i = 0; i < Polynomials.size(); i++) {
					cout << "P(" << i << ")= ";
					Polynomials[i].showPoly();
					cout << endl;
				}
			}
		}
		else if (choice == 'e' || choice == 'E') {
			if (Polynomials.size() == 0) cout << "\nYou must input a polynomial first!\n";
			else {
				cout << "\nChoose a method: [B]rute, [H]orn's, [S]parse, [A]ll of them, or let program [D]ecide? ";
				cin >> choice;
				cin.clear();
				cin.ignore(100, '\n');
				if (choice == 'b' || choice == 'B') {
					cout << "Which Polynomial? Choose 0-" << Polynomials.size() - 1 << " ";
					cin >> polyIn;
					cin.clear();
					cin.ignore(100, '\n');
					if (polyIn >= 0 && polyIn < Polynomials.size()) {
						cout << "What is x? (Choose small for very large exponents)";
						cin >> eval;
						cin.clear();
						cin.ignore(100, '\n');
						auto start = std::chrono::high_resolution_clock::now();
						retMe = Polynomials[polyIn].brute(eval);
						auto elapsed = std::chrono::high_resolution_clock::now() - start;
						long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
						cout << "\nThe time it took to calculate with Brute was " << microseconds << " microseconds.\n";
						cout << "The Brute value was: " << retMe << endl;
					}
					else {
						cout << "Sorry, that wasn't a valid Polynomial. Quitting to menu.";
					}
				}
				else if (choice == 'h' || choice == 'H') {
					cout << "Which Polynomial? Choose 0-" << Polynomials.size() - 1 << " ";
					cin >> polyIn;
					cin.clear();
					cin.ignore(100, '\n');
					if (polyIn >= 0 && polyIn < Polynomials.size()) {
						cout << "What is x? (Choose small for very large exponents)";
						cin >> eval;
						cin.clear();
						cin.ignore(100, '\n');
						auto start = std::chrono::high_resolution_clock::now();
						retMe = Polynomials[polyIn].horn(eval);
						auto elapsed = std::chrono::high_resolution_clock::now() - start;
						long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
						cout << "\nThe time it took to calculate with Horn's was " << microseconds << " microseconds.\n";
						cout << "The Horn value was: " << retMe << endl;
					}
					else {
						cout << "Sorry, that wasn't a valid Polynomial. Quitting to menu.";
					}
				}
				else if (choice == 's' || choice == 'S') {
					cout << "Which Polynomial? Choose 0-" << Polynomials.size() - 1 << " ";
					cin >> polyIn;
					cin.clear();
					cin.ignore(100, '\n');
					if (polyIn >= 0 && polyIn < Polynomials.size()) {
						cout << "What is x? (Choose small for very large exponents)";
						cin >> eval;
						cin.clear();
						cin.ignore(100, '\n');
						auto start = std::chrono::high_resolution_clock::now();
						node * temp = Polynomials[polyIn].getHead();
						while (temp->next != nullptr) {
							temp = temp->next;
						}
						unsigned int log = temp->expo;
						int count = -1; //since binary numbers start at 2^0
						while (log > 0) {
							log >>= 1;
							count++;
						}
						retMe = Polynomials[polyIn].sparse(count, eval);
						auto elapsed = std::chrono::high_resolution_clock::now() - start;
						long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
						cout << "\nThe time it took to calculate with Sparse was " << microseconds << " microseconds.\n";
						cout << "The Sparse value was: " << retMe << endl;
					}
					else {
						cout << "Sorry, that wasn't a valid Polynomial. Quitting to menu.";
					}
				}
				else if (choice == 'a' || choice == 'A') {
					cout << "Which Polynomial? Choose 0-" << Polynomials.size() - 1 << " ";
					cin >> polyIn;
					cin.clear();
					cin.ignore(100, '\n');
					if (polyIn >= 0 && polyIn < Polynomials.size()) {
						cout << "What is x? (Choose small for very large exponents)";
						cin >> eval;
						cin.clear();
						cin.ignore(100, '\n');
						Polynomials[polyIn].testAll(eval);
					}
					else {
						cout << "Sorry, that wasn't a valid Polynomial. Quitting to menu.";
					}
				}
				else if (choice == 'd' || choice == 'D') {
					cout << "Which Polynomial? Choose 0-" << Polynomials.size() - 1 << " ";
					cin >> polyIn;
					cin.clear();
					cin.ignore(100, '\n');
					if (polyIn >= 0 && polyIn < Polynomials.size()) {
						cout << "What is x? (Choose small for very large exponents)";
						cin >> eval;
						cin.clear();
						cin.ignore(100, '\n');
						retMe = Polynomials[polyIn].decide(eval);
						cout << "The value was : " << retMe;
					}
					else {
						cout << "Sorry, that wasn't a valid Polynomial. Quitting to menu.";
					}
				}
				else {
					cout << "Sorry, that wasn't a valid choice. Quitting to menu.";
				}
			}
		}
		else if (choice == 'r' || choice == 'R') {
			if (Polynomials.size() == 0) cout << "\nYou must input a polynomial first!\n";
			else {
				cout << "\nEnter the coefficient and exponent to see if any Polynomial has it. ";
				cin >> eval >> polyIn;
				for (int i = 0; i < Polynomials.size(); i++) {
					if (Polynomials[i].search(eval, polyIn)) {
						Polynomials[i].showPoly();
						cout << "\nThe above Polynomial contains that Monomial!\n";
					}
				}
			}
		}
		else if (choice == 'f' || choice == 'F') {
			if (Polynomials.size() == 0) cout << "\nYou must input a polynomial first!\n";
			else {
				cout << "\nEnter the exponent to find all coefficients from the Polynomials. ";
				cin >> polyIn;
				for (int i = 0; i < Polynomials.size(); i++) {
					eval = Polynomials[i].findCoeff(polyIn);
					if (eval != 0) {
						cout << "\nThe coefficient for " << polyIn << " was " << eval << " in P(" << i << ")";
					}
				}
			}
		}
		else if (choice == 'a' || choice == 'A') {
			if (Polynomials.size() == 0) cout << "\nYou must input a polynomial first!\n";
			else {
				cout << "\nChoose two Polynomials: Choose 0-" << Polynomials.size() - 1 << " ";
				cin >> polyIn >> second;
				if (polyIn >= 0 && polyIn < Polynomials.size() && second >= 0 && second < Polynomials.size()) {
					cin.clear();
					cin.ignore(100, '\n');
					Polynomial temp = Polynomials[polyIn] + Polynomials[second];
					Polynomials.push_back(temp);
					cout << "\nThe sum is: \n";
					temp.showPoly();
				}
				else {
					cout << "Sorry, that wasn't a valid Polynomial. Quitting to menu.";
				}
			}
		}
		else if (choice == 'd' || choice == 'D') {
			if (Polynomials.size() == 0) cout << "\nYou must input a polynomial first!\n";
			else {
				cout << "\nChoose two Polynomials: Choose 0-" << Polynomials.size() - 1 << " ";
				cin >> polyIn >> second;
				if (polyIn >= 0 && polyIn < Polynomials.size() && second >= 0 && second < Polynomials.size()) {
					cin.clear();
					cin.ignore(100, '\n');
					Polynomial temp = Polynomials[polyIn] - Polynomials[second];
					Polynomials.push_back(temp);
					cout << "\nThe sum is: \n";
					temp.showPoly();
				}
				else {
					cout << "Sorry, that wasn't a valid Polynomial. Quitting to menu.";
				}
			}
		}
		else if (choice == 'u' || choice == 'U') {
			if (Polynomials.size() == 0) cout << "\nYou must input a polynomial first!\n";
			else {
				cout << "\nChoose two Polynomials: Choose 0-" << Polynomials.size() - 1 << " ";
				cin >> polyIn >> second;
				if (polyIn >= 0 && polyIn < Polynomials.size() && second >= 0 && second < Polynomials.size()) {
					cin.clear();
					cin.ignore(100, '\n');
					Polynomial temp = Polynomials[polyIn] * Polynomials[second];
					Polynomials.push_back(temp);
					cout << "\nThe sum is: \n";
					temp.showPoly();
				}
				else {
					cout << "Sorry, that wasn't a valid Polynomial. Quitting to menu.";
				}
			}
		}
		else if (choice == 'q' || choice == 'Q') {
			break;
		}
		else {
			cout << "Error, unidentified key. Please re-enter.\n";
		}
		cout << endl;
		system("pause");
	}
	cout << "\nQuitting. Have a nice day~\n";
	system("pause");
	return;
}