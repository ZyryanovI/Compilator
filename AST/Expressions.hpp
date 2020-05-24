#pragma once

#include "Interfaces.hpp"

class AndExpression : public IExpression {
public:
    std::unique_ptr<IExpression> expr1;
    std::unique_ptr<IExpression> expr2;

    AndExpression(int first_line, int first_column, IExpression *expr1, IExpression *expr2) {
    if (expr1 == nullptr || expr2 == nullptr) {
        std::cout << "Expression is null" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->expr1 = std::unique_ptr<IExpression>(expr1);
    this->expr2 = std::unique_ptr<IExpression>(expr2);
}

    void Accept(Visitor* visitor) const {
    visitor->visit(this);
};

class LessExpression : public IExpression {
public:
    std::unique_ptr<IExpression> expr1;
    std::unique_ptr<IExpression> expr2;

    LessExpression(int first_line, int first_column, IExpression *expr1, IExpression *expr2) {
    if (expr1 == nullptr || expr2 == nullptr) {
        std::cout << "Expression is null" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->expr1 = std::unique_ptr<IExpression>(expr1);
    this->expr2 = std::unique_ptr<IExpression>(expr2);
    }

    void Accept(Visitor* visitor) const {
    visitor->visit(this);
    }
};

class PlusExpression : public IExpression {
public:
    std::unique_ptr<IExpression> expr1;
    std::unique_ptr<IExpression> expr2;

    PlusExpression(int first_line, int first_column, IExpression *expr1, IExpression *expr2) {
    if (expr1 == nullptr || expr2 == nullptr) {
        std::cout << "Expression is null" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->expr1 = std::unique_ptr<IExpression>(expr1);
    this->expr2 = std::unique_ptr<IExpression>(expr2);
    }

    void Accept(Visitor* visitor) const {
    visitor->visit(this);
};

class MinusExpression : public IExpression {
public:
    std::unique_ptr<IExpression> expr1;
    std::unique_ptr<IExpression> expr2;

   MinusExpression(int first_line, int first_column, IExpression *expr1, IExpression *expr2) {
    if (expr1 == nullptr || expr2 == nullptr) {
        std::cout << "Expression is null" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->expr1 = std::unique_ptr<IExpression>(expr1);
    this->expr2 = std::unique_ptr<IExpression>(expr2);
    }
    
    void Accept(Visitor* visitor) const {
    visitor->visit(this);
    }
};

class MultExpression : public IExpression {
public:
    std::unique_ptr<IExpression> expr1;
    std::unique_ptr<IExpression> expr2;

    MultExpression(int first_line, int first_column, IExpression *expr1, IExpression *expr2) {
    if (expr1 == nullptr || expr2 == nullptr) {
        std::cout << "Expression is null" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->expr1 = std::unique_ptr<IExpression>(expr1);
    this->expr2 = std::unique_ptr<IExpression>(expr2);
}

    void Accept(Visitor* visitor) const {
    visitor->visit(this);
    }
};


class ArrayExpression : public IExpression {
public:
    std::unique_ptr<IExpression> expr1;
    std::unique_ptr<IExpression> expr2;

    ArrayExpression(int first_line, int first_column, IExpression *expr1, IExpression *expr2) {
    if (expr1 == nullptr || expr2 == nullptr) {
        std::cout << "Expression is null" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->expr1 = std::unique_ptr<IExpression>(expr1);
    this->expr2 = std::unique_ptr<IExpression>(expr2);
    }

   void Accept(Visitor* visitor) const {
   visitor->visit(this);
   }
};

class LengthExpression : public IExpression {
public:
    std::unique_ptr<IExpression> expr;

    LengthExpression(int first_line, int first_column, IExpression *expr) {
    if (expr == nullptr) {
        std::cout << "Expression is null" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->expr = std::unique_ptr<IExpression>(expr);
    }

    void Accept(Visitor* visitor) const {
    visitor->visit(this);
    }
};

class MethodExpression : public IExpression {
public:
    std::unique_ptr<IExpression> class_name;
    std::unique_ptr<IIdentifier> method;
    std::unique_ptr<std::vector<std::unique_ptr<IExpression>>> params;

MethodExpression(int first_line, int first_column, IExpression *class_name,
                                   IIdentifier *method, std::vector<std::unique_ptr<IExpression>> *params) {
    if (class_name == nullptr || method == nullptr || params == nullptr) {
        std::cout << "While initializing MethodExpression, one of params is nullptr" <<std::endl;
    }

    this->first_line = first_line;
    this->first_column = first_column;
    this->class_name = std::unique_ptr<IExpression>(class_name);
    this->method = std::unique_ptr<IIdentifier>(method);
    this->params = std::unique_ptr<std::vector<std::unique_ptr<IExpression>>>(params);
}

void Accept(Visitor* visitor) const {
    visitor->visit(this);
}
};

class Integer : public IExpression {
public:
    int num;

    Integer(int first_line, int first_column, int num);

    void Accept(Visitor* visitor) const {
    visitor->visit(this);
    }
};

class Bool : public IExpression {
public:
    bool b;

    Bool(int first_line, int first_column, bool b) : b(b) {
    this->first_line = first_line;
    this->first_column = first_column;
    }

    void Accept(Visitor* visitor) const {
    visitor->visit(this);
    }
};

class IdentExpression : public IExpression {
public:

std::unique_ptr<IIdentifier> ident;
IdentExpression(int first_line, int first_column, IIdentifier *ident) {
    if (ident == nullptr) {
        std::cout << "While initializing IdentExpression, one of params is nullptr" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->ident = std::unique_ptr<IIdentifier>(ident);
}

void Accept(Visitor* visitor) const {
    visitor->visit(this);
}
};

class This : public IExpression {
public:
    
This(int first_line, int first_column) {
    this->first_line = first_line;
    this->first_column = first_column;
}

void Accept(Visitor* visitor) const {
    visitor->visit(this);
}
    
};

class NewArrExpression : public IExpression {
public:
    std::unique_ptr<IExpression> expr;

NewArrExpression(int first_line, int first_column, IExpression *expr) {
    if (expr == nullptr) {
        std::cout << "While initializing NewArrExpression, one of params is nullptr" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->expr = std::unique_ptr<IExpression>(expr);
}
    
void Accept(Visitor* visitor) const {
    visitor->visit(this);
}
    
};

class NewExpression : public IExpression {
public:

std::unique_ptr<IIdentifier> ident;

NewExpression(int first_line, int first_column, IIdentifier *ident) {
    if (ident == nullptr) {
        std::cout << "While initializing NewExpression, one of params is nullptr" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->ident = std::unique_ptr<IIdentifier>(ident);
}

void Accept(Visitor* visitor) const {
    visitor->visit(this);
}

};

class NotExpression : public IExpression {
public:
    std::unique_ptr<IExpression> expr;

NotExpression(int first_line, int first_column, IExpression *expr) {
    if (expr == nullptr) {
        std::cout << "While initializing NotExpression, one of params is nullptr" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->expr = std::unique_ptr<IExpression>(expr);
}

void Accept(Visitor* visitor) const {
    visitor->visit(this);
}
};

class Expression : public IExpression {
public:
    std::unique_ptr<IExpression> expr;

Expression(int first_line, int first_column, IExpression *expr) {
    if (expr == nullptr) {
        std::cout << "While initializing Expression, one of params is nullptr" <<std::endl;
    }
    this->first_line = first_line;
    this->first_column = first_column;
    this->expr = std::unique_ptr<IExpression>(expr);
}

void Accept(Visitor* visitor) const {
    visitor->visit(this);
}

};

