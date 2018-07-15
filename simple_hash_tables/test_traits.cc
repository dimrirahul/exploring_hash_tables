#include <iostream>
#include <memory>

using namespace std;

/**
 * The idea is simple I want to instantiate a template.
 * The template class should be exactly from a class that extends from a certain
 * base class.
 * For example there is a base class 'B' that has pure virtual method 'getKey'.
 * I want to use this is a template which should only accept classes that
 * derive from 'B'. Java has something like < T extends B >.
 */
using KeyType = int64_t;
struct BaseClass {
        virtual KeyType getKey() const = 0;
        virtual ~BaseClass() {
        }
};

struct C : public BaseClass {
        KeyType getKey() const {
                return 54;
        }
};

template<typename D, typename B>
struct IsDerivedTest {
        static void constraintChecker(D* dPtr) {
                B* bPtr = dPtr;
                (void)bPtr;
        }

        IsDerivedTest() {
                void (*p)(D*) = constraintChecker;
                (void)p;
        }

};

/*
template<typename D>
struct IsDerivedTest<D, void> {
        IsDerivedTest() {
                char* p = (int*) 0;
        }
};
*/

template <typename D>
struct KeyPrinter: IsDerivedTest<D, BaseClass> {

        void printKey(BaseClass* d) {
                cout << d->getKey() << "\n";
        }
};

struct NotDerived {
        KeyType getKey() const {
                return 20;
        }
};

int main() {
        KeyPrinter<C> keyPrinter;
        C c;
        keyPrinter.printKey(&c);
        NotDerived nd;
        KeyPrinter<NotDerived> kp1;
        kp1.printKey(&nd);
        return 0;
}
