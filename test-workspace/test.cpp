#include <iostream>
#include <vector>
#include <memory>
#include <functional>

// --- Keyword: using directive ---
using namespace std; // Scope: keyword.other.using.directive.cpp -> Expected: Keyword (#98676a)

namespace TestNamespace
{
    class MyClass; // Forward declaration
}

class Base
{
public:
    // --- Keyword: virtual specifier ---
    // Scope: storage.modifier.specifier -> Expected: Keyword (#98676a)
    virtual void baseMethod()
    {
        cout << "Base method" << endl;
    }
    // Scope: keyword.other.default.constructor.cpp -> Expected: Keyword (#98676a)
    virtual ~Base() = default;
};

// --- Keyword: final specifier ---
// Scope: storage.modifier.specifier -> Expected: Keyword (#98676a)
class Derived final : public Base
{
public:
    int value;

    // --- Keyword: explicit specifier ---
    // Scope: storage.modifier.specifier -> Expected: Keyword (#98676a)
    explicit Derived(int v) : Base(), value(v) {} // CORRECTED Initializer list

    // --- Keyword: override specifier ---
    // Scope: storage.modifier.specifier -> Expected: Keyword (#98676a)
    void baseMethod() override
    {
        cout << "Derived override" << endl;
        // Comment removed from here
        this->value = 10; // Scope: variable.language.this.cpp -> Expected: Red (#dc3958)
    }

    // --- Keyword: operator overload ---
    // Scope: keyword.other.operator.overload.cpp -> Expected: Keyword (#98676a)
    Derived &amp;
    operator+(const Derived other)
    {
        // Comment removed from here
        this->value += other.value;
        return *this;
    }
};

// --- Function Parameter ---
// Scope: variable.parameter.cpp -> Expected: Beige (#d3af86)
void processVector(const vector<int> &amp; data)
{
    // Scope: variable.parameter.function (check grammar) -> Expected: Beige (#d3af86)
    for (int val : data)
    {
        cout << val << " ";
    }
    cout << endl;
}

// --- Keyword: inline specifier ---
// Scope: storage.modifier.specifier -> Expected: Keyword (#98676a)
inline int add(int a, int b)
{
    return a + b;
}

// --- Keyword: constexpr specifier ---
// Scope: storage.modifier.specifier -> Expected: Keyword (#98676a)
constexpr int compileTimeValue(int x)
{
    return x * 2;
}

// --- Storage: Function Pointer Type ---
// Scope: storage.type.function.pointer.cpp -> Expected: Keyword (#98676a)
typedef void (*FuncPtr)(int);

void exampleCallback(int id)
{
    cout << "Callback called with ID: " << id << endl;
}

int main()
{
    vector<int> myVector = {1, 2, 3};
    // Scope: variable.parameter.cpp -> Expected: Beige (#d3af86) (on myVector usage in call)
    processVector(myVector); // Function call

    // --- Keyword: Memory Allocation (new/delete) ---
    // Scope: keyword.operator.new.cpp -> Expected: Keyword (#98676a)
    Derived *d_ptr = new Derived(5);
    // Scope: keyword.operator.new.cpp -> Expected: Keyword (#98676a)
    Base *b_ptr = new Derived(10);

    d_ptr->baseMethod();
    b_ptr->baseMethod();

    // --- Keyword: Cast Operators ---
    // Scope: keyword.operator.cast.dynamic_cast.cpp -> Expected: Keyword (#98676a)
    Derived *cast_d_ptr = dynamic_cast<Derived *>(b_ptr);
    if (cast_d_ptr)
    {
        cout << "Dynamic cast successful. Value: " << cast_d_ptr->value << endl;
    }

    const Derived *const_d_ptr = d_ptr;
    // Scope: keyword.operator.cast.const_cast.cpp -> Expected: Keyword (#98676a)
    Derived *non_const_ptr = const_cast<Derived *>(const_d_ptr);
    non_const_ptr->value = 20;

    // Scope: keyword.operator.cast.static_cast.cpp -> Expected: Keyword (#98676a)
    void *void_ptr = static_cast<void *>(d_ptr);
    // Scope: keyword.operator.cast.reinterpret_cast.cpp -> Expected: Keyword (#98676a)
    Derived *reint_ptr = reinterpret_cast<Derived *>(void_ptr);
    cout << "Reinterpret cast value: " << reint_ptr->value << endl;

    // Scope: keyword.operator.delete.cpp -> Expected: Keyword (#98676a)
    delete d_ptr;
    // Scope: keyword.operator.delete.cpp -> Expected: Keyword (#98676a)
    delete b_ptr;

    // --- Constant: nullptr ---
    // Scope: constant.language.nullptr.cpp -> Expected: Orange (#f79a32)
    Derived *null_obj = nullptr;
    if (null_obj == nullptr)
    {
        cout << "Pointer is null." << endl;
    }

    // --- Punctuation: Scope Resolution ---
    // Scope: punctuation.separator.scope-resolution.cpp -> Expected: Beige (#d3af86)
    std::cout << "Scope resolution test." << std::endl;

    // --- Storage: Function Pointer Usage ---
    // Scope: storage.type.function.pointer.cpp (on FuncPtr type usage)
    FuncPtr callback = exampleCallback;
    callback(123);

    // --- Storage: Lambda mutable ---
    int counter = 0;
    // Scope: storage.modifier.lambda.mutable.cpp -> Expected: Keyword (#98676a)
    auto myLambda = [counter]() mutable
    {
        counter++;
        cout << "Lambda counter: " << counter << endl;
    };
    myLambda();
    myLambda(); // See if mutable allows change

    // Scope: storage.modifier.specifier (on constexpr usage)
    constexpr int val = compileTimeValue(10);
    cout << "Compile time value: " << val << endl;

    return 0;
}

// Definition for namespace class
namespace TestNamespace
{
    class MyClass
    {
    public:
        int member;
        static void staticMethod()
        { // Example static member
            std::cout << "Static method in namespace" << std::endl;
        }
    };
}

void namespaceTest()
{
    // --- Punctuation: Namespace Access ---
    // Scope: punctuation.separator.namespace.access -> Expected: Beige (#d3af86)
    TestNamespace::MyClass obj;
    obj.member = 5;
    // --- Punctuation: Scope Resolution (Static Member) ---
    // Scope: punctuation.separator.scope-resolution.cpp -> Expected: Beige (#d3af86)
    TestNamespace::MyClass::staticMethod();
}