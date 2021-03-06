//
//  FunctionCodeGenerator.hpp
//  Emojicode
//
//  Created by Theo Weidmann on 29/07/2017.
//  Copyright © 2017 Theo Weidmann. All rights reserved.
//

#ifndef FunctionCodeGenerator_hpp
#define FunctionCodeGenerator_hpp

#include "CodeGenerator.hpp"
#include "Functions/Function.hpp"
#include "Scoping/IDScoper.hpp"
#include <llvm/IR/IRBuilder.h>
#include <functional>

namespace EmojicodeCompiler {

class Compiler;

/// A FunctionCodeGenerator instance is responsible for generating the LLVM IR for a single function.
class FunctionCodeGenerator {
public:
    /// Constructs a FunctionCodeGenerator.
    FunctionCodeGenerator(Function *function, llvm::Function *llvmFunc, CodeGenerator *generator)
    : fn_(function), function_(llvmFunc), scoper_(function->variableCount()),
      generator_(generator), builder_(generator->context()) {}
    void generate();

    CGScoper& scoper() { return scoper_; }
    Compiler* compiler() const;
    CodeGenerator* generator() const { return generator_; }
    llvm::IRBuilder<>& builder() { return builder_; }
    LLVMTypeHelper& typeHelper() { return generator()->typeHelper(); }
    virtual llvm::Value* thisValue() const { return &*function_->args().begin(); }
    llvm::Type* llvmReturnType() const { return function_->getReturnType(); }

    /// @returns The number of bytes an instance of @c type takes up in memory.
    /// @see sizeOfReferencedType
    llvm::Value* sizeOf(llvm::Type *type);
    /// @returns The number of bytes an instance of the type pointed to by a pointer of @c ptrType takes up.
    ///          Behaves like `sizeOf(ptrType->getElementType())`, although this method does not call sizeOf().
    /// @see sizeOf
    llvm::Value* sizeOfReferencedType(llvm::PointerType *ptrType);
    /// Gets a pointer to the box info field of box.
    /// @param box A pointer to a box.
    llvm::Value* getBoxInfoPtr(llvm::Value *box);
    llvm::Value* getValuePtr(llvm::Value *box, const Type &type);
    llvm::Value* getValuePtr(llvm::Value *box, llvm::Type *llvmType);
    llvm::Value* getMakeNoValue(llvm::Value *box);

    llvm::Value* getHasNoValue(llvm::Value *simpleOptional);
    llvm::Value* getHasNoValueBox(llvm::Value *box);
    llvm::Value* getHasNoValueBoxPtr(llvm::Value *box);
    llvm::Value* getSimpleOptionalWithoutValue(const Type &type);
    llvm::Value* getBoxOptionalWithoutValue();
    llvm::Value* getSimpleOptionalWithValue(llvm::Value *value, const Type &type);

    /// Gets a pointer to the pointer to the class info of an object.
    /// @see getClassInfoFromObject
    llvm::Value* getClassInfoPtrFromObject(llvm::Value *object);
    /// Gets a pointer to the class info of an object.
    /// @param object Pointer to the object from which the class info shall be obtained.
    /// @returns A llvm::Value* representing a pointer to a class info.
    llvm::Value* getClassInfoFromObject(llvm::Value *object);

    llvm::Value* getErrorNoError() { return int64(-1); }
    llvm::Value* getIsError(llvm::Value *simpleError);
    llvm::Value* getSimpleErrorWithError(llvm::Value *errorEnumValue, llvm::Type *type);
    llvm::Value* getErrorEnumValueBoxPtr(llvm::Value *box, const Type &type);

    llvm::Value* int8(int8_t value);
    llvm::Value* int16(int16_t value);
    llvm::Value* int32(int32_t value);
    llvm::Value* int64(int64_t value);

    llvm::Value* boxInfoFor(const Type &type);

    llvm::Value* alloc(llvm::PointerType *type);

    llvm::Value* createEntryAlloca(llvm::Type *type, const llvm::Twine &name = "");

    void createIfElse(llvm::Value* cond, const std::function<void()> &then, const std::function<void()> &otherwise);
    void createIfElseBranchCond(llvm::Value* cond, const std::function<bool()> &then,
                                const std::function<bool()> &otherwise);
    llvm::Value* createIfElsePhi(llvm::Value* cond, const std::function<llvm::Value* ()> &then,
                                 const std::function<llvm::Value *()> &otherwise);

    using PairIfElseCallback = std::function<std::pair<llvm::Value*, llvm::Value*> ()>;
    std::pair<llvm::Value*, llvm::Value*> createIfElsePhi(llvm::Value* cond, const PairIfElseCallback &then,
                                                          const PairIfElseCallback &otherwise);
protected:
    virtual void declareArguments(llvm::Function *function);
    Function* function() const { return fn_; }
private:
    Function *const fn_;
    llvm::Function *const function_;
    CGScoper scoper_;

    CodeGenerator *const generator_;
    llvm::IRBuilder<> builder_;

    void addParamAttrs(const Type &argType, llvm::Argument &llvmArg);
};

}  // namespace EmojicodeCompiler

#endif /* FunctionCodeGenerator_hpp */
