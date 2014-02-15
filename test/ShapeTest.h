#include "Test.h"
#include "../src/Shape.h"

class ShapeTest : public Test {
    protected:
        // This is defined by the subclasses.
        virtual Test* createShape() const = 0;
        virtual int testConstructor(Test*) = 0;
        virtual int testBounds(real, real, real, real, real, real) = 0;
        
        Array<TraceRes> trace(const Ray&) const {
            
        }

        struct RayTest {
            Ray ray;
            TraceRes desired;
        };
    public:
        int perform() {
            std::cout << "Performing " << note << "Correctness Tests" << norm << " :" << std::endl;
            int errors = 0;

            // Create the shape.
            Test *shape = createShape();

            // Check that the shape was constructed correctly.
            errors += testConstructor(shape);

            // Perform various intersection tests.
            
        }

        virtual std::string name() const = 0;
};

