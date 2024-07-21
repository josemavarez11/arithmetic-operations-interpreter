#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>
#include <cmath>
#include <stdexcept>

enum class Estado { INICIO, DIGITO, LETRA, PUNTO, ERROR, ACEPTACION };

class Automata {
public:
    Automata() = default;
    bool validarDigito(const std::string& cadena);
    bool validarLetra(const std::string& cadena);
    bool validarPalabra(const std::string& cadena);
    bool validarNumeroEntero(const std::string& cadena);
    bool validarNumeroReal(const std::string& cadena);
    bool validarIdentificador(const std::string& cadena);
    bool validarOperador(const std::string& cadena);
};

bool Automata::validarDigito(const std::string& cadena) {
    return cadena.length() == 1 && isdigit(cadena[0]);
}

bool Automata::validarLetra(const std::string& cadena) {
    return cadena.length() == 1 && isalpha(cadena[0]);
}

bool Automata::validarPalabra(const std::string& cadena) {
    Estado estado = Estado::INICIO;
    for (char c : cadena) {
        switch (estado) {
            case Estado::INICIO:
                if (isalpha(c)) {
                    estado = Estado::LETRA;
                } else {
                    estado = Estado::ERROR;
                }
                break;
            case Estado::LETRA:
                if (isalpha(c)) {
                    estado = Estado::LETRA;
                } else {
                    estado = Estado::ERROR;
                }
                break;
            default:
                return false;
        }
    }
    return estado == Estado::LETRA;
}

bool Automata::validarNumeroEntero(const std::string& cadena) {
    Estado estado = Estado::INICIO;
    for (char c : cadena) {
        switch (estado) {
            case Estado::INICIO:
                if (isdigit(c)) {
                    estado = Estado::DIGITO;
                } else {
                    estado = Estado::ERROR;
                }
                break;
            case Estado::DIGITO:
                if (isdigit(c)) {
                    estado = Estado::DIGITO;
                } else {
                    estado = Estado::ERROR;
                }
                break;
            default:
                return false;
        }
    }
    return estado == Estado::DIGITO;
}

bool Automata::validarNumeroReal(const std::string& cadena) {
    Estado estado = Estado::INICIO;
    for (char c : cadena) {
        switch (estado) {
            case Estado::INICIO:
                if (isdigit(c)) {
                    estado = Estado::DIGITO;
                } else {
                    estado = Estado::ERROR;
                }
                break;
            case Estado::DIGITO:
                if (isdigit(c)) {
                    estado = Estado::DIGITO;
                } else if (c == '.') {
                    estado = Estado::PUNTO;
                } else {
                    estado = Estado::ERROR;
                }
                break;
            case Estado::PUNTO:
                if (isdigit(c)) {
                    estado = Estado::ACEPTACION;
                } else {
                    estado = Estado::ERROR;
                }
                break;
            case Estado::ACEPTACION:
                if (isdigit(c)) {
                    estado = Estado::ACEPTACION;
                } else {
                    estado = Estado::ERROR;
                }
                break;
            default:
                return false;
        }
    }
    return estado == Estado::ACEPTACION;
}

bool Automata::validarIdentificador(const std::string& cadena) {
    Estado estado = Estado::INICIO;
    for (char c : cadena) {
        switch (estado) {
            case Estado::INICIO:
                if (isalpha(c) || c == '_') {
                    estado = Estado::LETRA;
                } else {
                    estado = Estado::ERROR;
                }
                break;
            case Estado::LETRA:
                if (isalnum(c) || c == '_') {
                    estado = Estado::LETRA;
                } else {
                    estado = Estado::ERROR;
                }
                break;
            default:
                return false;
        }
    }
    return estado == Estado::LETRA;
}

bool Automata::validarOperador(const std::string& cadena) {
    return cadena.length() == 1 && (cadena == "+" || cadena == "-" || cadena == "*" || cadena == "/" || cadena == "%" || cadena == "^");
}

enum class TokenType { IDENTIFICADOR, NUMERO_ENTERO, NUMERO_REAL, OPERADOR, PARENTESIS_IZQ, PARENTESIS_DER, ASIGNACION, FUNCION, IMPRIMIR, ERROR };

struct Token {
    TokenType tipo;
    std::string valor;
    int linea;
};

class Lexer {
public:
    Lexer(const std::string& input) : input(input), position(0), line(1) {}
    std::vector<Token> tokenize();
private:
    std::string input;
    size_t position;
    int line;
    char peek();
    char advance();
    void skipWhitespace();
    Token number();
    Token identifier();
};

char Lexer::peek() {
    return position < input.size() ? input[position] : '\0';
}

char Lexer::advance() {
    if (position < input.size()) {
        if (input[position] == '\n') line++;
        return input[position++];
    }
    return '\0';
}

void Lexer::skipWhitespace() {
    while (isspace(peek())) advance();
}

Token Lexer::number() {
    std::string value;
    bool isReal = false;
    while (isdigit(peek()) || peek() == '.') {
        if (peek() == '.') isReal = true;
        value += advance();
    }
    if (isReal) {
        return {TokenType::NUMERO_REAL, value, line};
    } else {
        return {TokenType::NUMERO_ENTERO, value, line};
    }
}

Token Lexer::identifier() {
    std::string value;
    while (isalnum(peek()) || peek() == '_') {
        value += advance();
    }
    if (value == "print" || value == "root") {
        return {TokenType::FUNCION, value, line};
    } else {
        return {TokenType::IDENTIFICADOR, value, line};
    }
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (position < input.size()) {
        skipWhitespace();
        char current = peek();
        if (isdigit(current)) {
            tokens.push_back(number());
        } else if (isalpha(current) || current == '_') {
            tokens.push_back(identifier());
        } else if (current == '+') {
            tokens.push_back({TokenType::OPERADOR, std::string(1, advance()), line});
        } else if (current == '-') {
            tokens.push_back({TokenType::OPERADOR, std::string(1, advance()), line});
        } else if (current == '*') {
            tokens.push_back({TokenType::OPERADOR, std::string(1, advance()), line});
        } else if (current == '/') {
            tokens.push_back({TokenType::OPERADOR, std::string(1, advance()), line});
        } else if (current == '%') {
            tokens.push_back({TokenType::OPERADOR, std::string(1, advance()), line});
        } else if (current == '^') {
            tokens.push_back({TokenType::OPERADOR, std::string(1, advance()), line});
        } else if (current == '=') {
            tokens.push_back({TokenType::ASIGNACION, std::string(1, advance()), line});
        } else if (current == '(') {
            tokens.push_back({TokenType::PARENTESIS_IZQ, std::string(1, advance()), line});
        } else if (current == ')') {
            tokens.push_back({TokenType::PARENTESIS_DER, std::string(1, advance()), line});
        } else if (current == '\0') {
            break;
        } else {
            std::string value(1, advance());
            tokens.push_back({TokenType::ERROR, value, line});
        }
    }
    return tokens;
}

class Parser {
public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens), position(0) {}
    void parse();
private:
    std::vector<Token> tokens;
    size_t position;
    Token advance();
    Token peek();
    bool match(TokenType type);
    double expression();
    double exponent();
    double term();
    double factor();
    void printStatement();
    void assignment();
    double function();
    double evaluate(const std::string& var);
    std::unordered_map<std::string, double> variables;
};

Token Parser::advance() {
    if (position < tokens.size()) {
        Token current = tokens[position++];
        return current;
    }
    return Token{TokenType::ERROR, "", 0};
}

Token Parser::peek() {
    if (position < tokens.size()) {
        Token current = tokens[position];
        return current;
    }
    return Token{TokenType::ERROR, "", 0};
}


bool Parser::match(TokenType type) {
    if (peek().tipo == type) {
        return true;
    }
    return false;
}

double Parser::expression() {
    double result = exponent();
    while (match(TokenType::OPERADOR) && (peek().valor == "+" || peek().valor == "-")) {
        Token op = advance();
        double right = exponent();
        if (op.valor == "+") result += right;
        else if (op.valor == "-") result -= right;
    }
    return result;
}

double Parser::exponent() {
    double result = term();
    while (match(TokenType::OPERADOR) && peek().valor == "^") {
        Token op = advance();
        double right = term();
        result = std::pow(result, right);
    }
    return result;
}

double Parser::term() {
    double result = factor();
    while (match(TokenType::OPERADOR) && (peek().valor == "*" || peek().valor == "/" || peek().valor == "%")) {
        Token op = advance();
        double right = factor();
        if (op.valor == "*") result *= right;
        else if (op.valor == "/") result /= right;
        else if (op.valor == "%") result = std::fmod(result, right);
    }
    return result;
}

double Parser::factor() {
    if (peek().tipo == TokenType::NUMERO_ENTERO || peek().tipo == TokenType::NUMERO_REAL) {
        Token token = advance();
        try {
            return std::stod(token.valor);
        } catch (const std::invalid_argument& e) {
            throw std::runtime_error("Error: Conversión invalida en la linea " + std::to_string(token.linea));
        }
    } else if (peek().tipo == TokenType::IDENTIFICADOR) {
        Token token = advance();
        if (variables.find(token.valor) != variables.end()) {
            return variables[token.valor];
        } else {
            throw std::runtime_error("Error: Variable no definida: " + token.valor);
        }
    } else if (peek().tipo == TokenType::PARENTESIS_IZQ) {
        advance(); // Consumir '('
        double result = expression();
        if (!match(TokenType::PARENTESIS_DER)) {
            throw std::runtime_error("Error sintactico: Falta ')' en la linea " + std::to_string(peek().linea));
        }
        advance(); // Consumir ')'
        return result;
    } else {
        throw std::runtime_error("Error sintactico en la linea " + std::to_string(peek().linea));
    }
}

void Parser::printStatement() {
    if (!match(TokenType::PARENTESIS_IZQ)) {
        throw std::runtime_error("Error sintactico: Falta '(' en la linea " + std::to_string(peek().linea));
    }
    advance(); // Consumir '('
    double result = expression();
    if (!match(TokenType::PARENTESIS_DER)) {
        throw std::runtime_error("Error sintactico: Falta ')' en la linea " + std::to_string(peek().linea));
    }
    advance(); // Consumir ')'
    std::cout << result << std::endl;
}

void Parser::assignment() {
    std::string var = advance().valor; // Consumir el identificador
    if (!match(TokenType::ASIGNACION)) {
        throw std::runtime_error("Error sintactico: Falta '=' en la linea " + std::to_string(peek().linea));
    }
    advance(); // Consumir '='
    double result;
    if (match(TokenType::FUNCION)) {
        result = function(); // Manejar la función dentro de la asignación
    } else {
        result = expression(); // Evaluar la expresión a la derecha de '='
    }
    variables[var] = result; // Almacenar el resultado en la variable
}


double Parser::function() {
    std::string func = advance().valor; // Consumir el nombre de la función
    if (!match(TokenType::PARENTESIS_IZQ)) {
        throw std::runtime_error("Error sintactico: Falta '(' en la linea " + std::to_string(peek().linea));
    }
    advance(); // Consumir '('
    double result = expression(); // Evaluar la expresión dentro de la función
    if (!match(TokenType::PARENTESIS_DER)) {
        throw std::runtime_error("Error sintactico: Falta ')' en la linea " + std::to_string(peek().linea));
    }
    advance(); // Consumir ')'
    if (func == "root") {
        result = std::sqrt(result);
        return result;
    } else {
        throw std::runtime_error("Error: Funcion no definida: " + func);
    }
}

double Parser::evaluate(const std::string& var) {
    if (variables.find(var) != variables.end()) {
        return variables[var];
    }
    throw std::runtime_error("Error: Variable no definida: " + var);
}

void Parser::parse() {
    while (position < tokens.size()) {
        Token currentToken = peek();

        if (currentToken.tipo == TokenType::IDENTIFICADOR) {
            assignment();
        } else if (currentToken.tipo == TokenType::FUNCION && currentToken.valor == "print") {
            advance();
            printStatement();
        } else if (currentToken.tipo == TokenType::FUNCION) {
            function();
        } else {
            throw std::runtime_error("Error sintactico en la linea " + std::to_string(currentToken.linea));
        }
    }
}

int main() {
    std::ifstream file("code.txt");
    if (!file) {
        std::cerr << "Error: No se pudo abrir el archivo code.txt" << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string code = buffer.str();

    Lexer lexer(code);
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    try {
        parser.parse();
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}