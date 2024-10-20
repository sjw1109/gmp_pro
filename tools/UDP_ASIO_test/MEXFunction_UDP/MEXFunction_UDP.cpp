/* ========================================================================
 * phonebook.cpp
 * example for illustrating how to manipulate structure.
 *
 * takes a (MxN) structure matrix which has first field as
 * character array(name), and second field as scalar double (phone number).
 * This function returns a new structure (1x1)containing following fields:
 * for character array input, it will be (MxN) cell array;
 * and for numeric double (noncomplex, scalar) input, it will be (MxN)
 * cell array where each field is numeric array of type double.
 *
 * Build : from MATLAB
 *         >> mex phonebook.cpp
 * Usage with example : from MATLAB
 *         >> friends(1).name = 'Jordan Robert';
 *         >> friends(1).phone = 3386;
 *         >> friends(2).name = 'Mary Smith';
 *         >> friends(2).phone = 3912;
 *         >> friends(3).name = 'Stacy Flora';
 *         >> friends(3).phone = 3238;
 *         >> friends(4).name = 'Harry Alpert';
 *         >> friends(4).phone = 3077;
 *         >> phonebook(friends)
 *
 * This is a MEX-file for MATLAB.
 * Copyright 2017 The MathWorks, Inc.
 *=======================================================================*/

#include "mex.hpp"
#include "mexAdapter.hpp"
#include <memory>
#include <string>


class MexFunction : public matlab::mex::Function
{
  private:
    std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr;

  public:
    /* Constructor for the class. */
    MexFunction()
    {
        matlabPtr = getEngine();
    }

    /* Helper function to print output string on MATLAB command prompt. */
    void displayOnMATLAB(std::ostringstream stream)
    {
        matlab::data::ArrayFactory factory;
        matlabPtr->feval(u"fprintf", 0, std::vector<matlab::data::Array>({factory.createScalar(stream.str())}));
    }

    void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs)
    {
        checkArguments(outputs, inputs);
        double multiplier = inputs[0][0];
        matlab::data::TypedArray<double> in = std::move(inputs[1]);
        arrayProduct(in, multiplier);
        outputs[0] = std::move(in);
    }

    void arrayProduct(matlab::data::TypedArray<double> &inMatrix, double multiplier)
    {

        for (auto &elem : inMatrix)
        {
            elem *= multiplier;
        }
    }

    void checkArguments(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs)
    {
        std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();
        matlab::data::ArrayFactory factory;

        if (inputs.size() != 2)
        {
            matlabPtr->feval(u"error", 0,
                             std::vector<matlab::data::Array>({factory.createScalar("Two inputs required")}));
        }

        if (inputs[0].getNumberOfElements() != 1)
        {
            matlabPtr->feval(
                u"error", 0,
                std::vector<matlab::data::Array>({factory.createScalar("Input multiplier must be a scalar")}));
        }

        if (inputs[0].getType() != matlab::data::ArrayType::DOUBLE ||
            inputs[0].getType() == matlab::data::ArrayType::COMPLEX_DOUBLE)
        {
            matlabPtr->feval(u"error", 0,
                             std::vector<matlab::data::Array>(
                                 {factory.createScalar("Input multiplier must be a noncomplex scalar double")}));
        }

        if (inputs[1].getType() != matlab::data::ArrayType::DOUBLE ||
            inputs[1].getType() == matlab::data::ArrayType::COMPLEX_DOUBLE)
        {
            matlabPtr->feval(
                u"error", 0,
                std::vector<matlab::data::Array>({factory.createScalar("Input matrix must be type double")}));
        }

        if (inputs[1].getDimensions().size() != 2)
        {
            matlabPtr->feval(
                u"error", 0,
                std::vector<matlab::data::Array>({factory.createScalar("Input must be m-by-n dimension")}));
        }
    }

};
