#include <iostream>

using namespace std;

// Data segment i initialized by the runtime to all bytes zero, except where
// explicit initializations set another value.

int count_calls() {
        // static local variable - works same as in C

        // Values scope is limited to this function; lifetime is lifetime of
        // program
        static int calls = 0;

        return ++calls;
}

static int global_value; // Does not change lifetime. All globals have lifetime
                         // equal to program lifetime. Scope is limited to
                         // "compilation unit." "compilation unit" is a fancy
                         // word for "file." Declaring this an "extern" in
                         // another file would anger the compiler.
                         // Same behavior as in C

class count_instances {
      private:
        // "Class variable" - there is a single instance of num_instances for
        // the entire program. As opposed to an instance variable, where there
        // is an instance per instance of the class.
        // In data segment. like the C-style statics. Lifetime is lifetime of
        // program. Scope is global. Can be used without instance of class.
        static int num_instances;
        // This is only a definition, not a declaration.
        // This must be decleared outside of class body!

      public:
        count_instances() { num_instances++; }

        // Static class functions can be used without an instance of the class.
        // Global scope
        static int get_num_instances() { return num_instances; };
};

// Classes do not generate assembly. So to define a static we have to generate
// it outside.
int count_instances::num_instances;

// In C++(as well as in C), static variables declared at the global scope or
// within a function, if not explicitly initialized, are automatically
// initialized to zero

int main(int argc, char *argv[]) {
        int i;

        for (i = 0; i < 10; i++) {
                cout << count_calls() << endl;
        }

        (void)global_value; // Supress unused warning

        cout << count_instances::get_num_instances() << endl;

        count_instances ci;

        cout << count_instances::get_num_instances() << endl;

        return 0;
}
