#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <variant>

enum class TokenType {
    PUSH,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    POP,
    NUMBER,
    STRING,
    INVALID,
    LITERAL
};

struct Token {
    TokenType type;
    std::string value;
};

class Lexer {
public:
    Lexer(const std::string& input) : input(input), position(0) {}

    Token getNextToken() {
        while (position < input.length()) {
            if (isdigit(input[position])) {
                return readNumber();
            }
            else if (isalpha(input[position])) {
                return readKeyword();
            }
            else if (input[position] == '\"') {
                return readString();
            }
            else if (isspace(input[position])) {
                skipWhitespace();
                continue;
            }
            else if (input[position] == '+') {
                position++;
                return { TokenType::ADD, "+" };
            }
            else if (input[position] == '-') {
                position++;
                return { TokenType::SUB, "-" };
            }
            else if (input[position] == '*') {
                position++;
                return { TokenType::MUL, "*" };
            }
            else if (input[position] == '/') {
                position++;
                return { TokenType::DIV, "/" };
            }
            else if (input[position] == '%') {
                position++;
                return { TokenType::MOD, "%" };
            }
            else if (input[position] == ':') {
                position++;
                return { TokenType::POP, ":" };
            }
            else if (isalpha(input[position])) {
                position++;
                return { TokenType::LITERAL, input };
            }
            else {
                return { TokenType::INVALID, std::string(1, input[position++]) };
            }
        }

        return { TokenType::INVALID, "" };
    }

private:
    std::string input;
    std::size_t position;

    Token readNumber() {
        std::string value;
        while (position < input.length() && isdigit(input[position])) {
            value += input[position++];
        }
        return { TokenType::NUMBER, value };
    }

    Token readKeyword() {
        std::string value;
        while (position < input.length() && isalpha(input[position])) {
            value += input[position++];
        }
        if (value == "push") {
            return { TokenType::PUSH, "push" };
        }
        else if (value == "add") {
            return { TokenType::ADD, "add" };
        }
        else if (value == "sub") {
            return { TokenType::SUB, "sub" };
        }
        else if (value == "mul") {
            return { TokenType::MUL, "mul" };
        }
        else if (value == "div") {
            return { TokenType::DIV, "div" };
        }
        else if (value == "mod") {
            return { TokenType::MOD, "mod" };
        }
        else if (value == "pop") {
            return { TokenType::POP, "pop" };
        }
        else {
            return { TokenType::INVALID, value };
        }
    }

    Token readString() {
        std::string value;
        position++;
        while (position < input.length() && input[position] != '\"') {
            value += input[position++];
        }
        position++;
        return { TokenType::STRING, value };
    }

    void skipWhitespace() {
        while (position < input.length() && isspace(input[position])) {
            position++;
        }
    }
};

class Parser {
public:
    Parser(Lexer lexer) : lexer(lexer) {}

    void parse() {
        Token token = lexer.getNextToken();
        while (token.type != TokenType::INVALID) {
            if (token.type == TokenType::PUSH) {
                parsePush();
            }
            else if (token.type == TokenType::POP) {
                parsePop();
            }
            else if (token.type == TokenType::ADD) {
                parseAdd();
            }
            else if (token.type == TokenType::SUB) {
                parseSub();
            }
            else if (token.type == TokenType::MUL) {
                parseMul();
            }
            else if (token.type == TokenType::DIV) {
                parseDiv();
            }
            else {
                std::cout << "Invalid syntax" << std::endl;
                exit(EXIT_FAILURE);
            }
            token = lexer.getNextToken();
        }
    }

    void parsePush() {
        Token token = lexer.getNextToken();
        if (token.type != TokenType::LITERAL && token.type != TokenType::STRING) {
            std::cout << "Invalid syntax" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (token.type == TokenType::LITERAL) {
            int value = stoi(token.value);
            stack.push(value);
        }
        else {
            stack.push(token.value);
        }
    }

    void parsePop() {
        if (stack.empty()) {
            std::cout << "Stack is empty" << std::endl;
            exit(EXIT_FAILURE);
        }
        stack.pop();
    }

    void parseAdd() {
        if (std::holds_alternative<int>(stack.top())) {
            if (stack.size() < 2) {
                std::cout << "Not enough values on stack" << std::endl;
                exit(EXIT_FAILURE);
            }
            int operand2 = std::get<int>(stack.top());
            stack.pop();
            int operand1 = std::get<int>(stack.top());
            stack.pop();
            stack.push(operand1 + operand2);
        }
    }
    void parseSub() {
        if (std::holds_alternative<int>(stack.top())) {
            if (stack.size() < 2) {
                std::cout << "Not enough values on stack" << std::endl;
                exit(EXIT_FAILURE);
            }
            int operand2 = std::get<int>(stack.top());
            stack.pop();
            int operand1 = std::get<int>(stack.top());
            stack.pop();
            stack.push(operand1 - operand2);
        }
    }

    void parseMul() {
        if (std::holds_alternative<int>(stack.top())) {
            if (stack.size() < 2) {
                std::cout << "Not enough values on stack" << std::endl;
                exit(EXIT_FAILURE);
            }
            int operand2 = std::get<int>(stack.top());
            stack.pop();
            int operand1 = std::get<int>(stack.top());
            stack.pop();
            stack.push(operand1 * operand2);
        }
    }

    void parseDiv() {
        if (std::holds_alternative<int>(stack.top()) ){
            if (stack.size() < 2) {
                std::cout << "Not enough values on stack" << std::endl;
                exit(EXIT_FAILURE);
            }
            int operand2 = std::get<int>(stack.top());
            stack.pop();
            int operand1 = std::get<int>(stack.top());
            stack.pop();
            if (operand2 == 0) {
                std::cout << "Division by zero" << std::endl;
                exit(EXIT_FAILURE);
            }
            stack.push(operand1 / operand2);
        }
    }

private:
    Lexer lexer;
    std::stack<std::variant<int, std::string>> stack;
};

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: zinc <filename>" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        std::cout << "Failed to open file" << std::endl;
        return EXIT_FAILURE;
    }

    std::string inputString((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    Lexer lexer(inputString);
    Parser parser(lexer);
    parser.parse();

    return EXIT_SUCCESS;
}