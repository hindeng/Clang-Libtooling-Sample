#include "clang/Driver/Options.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "json.hpp"
#include "iostream"
#include <iomanip>
#include <fstream> 
#include <string.h>
#include <vector>
#include <fstream>



using namespace std;
using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;
using json = nlohmann::json;
using namespace std;

class varToChange {
public:
    varToChange(string funcName, string varName, string targetType) {
        _funcName = funcName;
        _varName = varName;
        _targetType = targetType;
    }
    
    string _funcName;
    string _varName;
    string _targetType;
};

Rewriter rewriter;
vector<varToChange> varList;

static llvm::cl::OptionCategory StatSampleCategory("Stat Sample");

class ExampleVisitor : public RecursiveASTVisitor<ExampleVisitor> {
private:
    ASTContext *astContext; // used for getting additional AST info

public:
    explicit ExampleVisitor(CompilerInstance *CI): astContext(&(CI->getASTContext())) {  // initialize private members
        rewriter.setSourceMgr(astContext->getSourceManager(), astContext->getLangOpts());
    }

    // virtual bool VisitFunctionDecl(FunctionDecl *func) {
    //     numFunctions++;
    //     string funcName = func->getNameInfo().getName().getAsString();
    //     if (funcName == "do_math") {
    //         rewriter.ReplaceText(func->getLocation(), funcName.length(), "add5");
    //         errs() << "** Rewrote function def: " << funcName << "\n";
    //     } 
    //     return true;
    // }

    virtual bool VisitVarDecl(VarDecl *var) {
    	if (var->isLocalVarDeclOrParm() && !var->isLocalVarDecl()) {
    		return true;
    	}
    
    	string typeName = var->getInit()->getType().getAsString();
        string varName = var->getNameAsString();
        FunctionDecl *func = dyn_cast<FunctionDecl>(var->getParentFunctionOrMethod());
        string funcName = func->getNameInfo().getName().getAsString();
        cout << "type: " << setw(15) << left  << typeName ;
        cout << "name: " << setw(15) << left << varName;
        cout << "function: "<< setw(15) << left  << funcName << endl;
        
      for (vector<varToChange>::iterator itr = varList.begin(); itr != varList.end(); itr++) {
        if (varName.compare(itr->_varName) == 0 && funcName.compare(itr->_funcName) == 0) {
            rewriter.ReplaceText(var->getLocStart(), typeName.length(), itr->_targetType);
            cout << "Change " << varName << " to " << itr->_targetType << endl;
            varList.erase(itr);
            break;
        }
      }

        
//         if (varName.compare("target") == 0) {
//             errs() << "** Found Target and Changed Type " << "\n";
//             rewriter.ReplaceText(var->getLocStart(), 10, "double");
//         }

    	//string varName = var->getInit()->getLHS();
    	//cout << varName  << "\n" ;
//     	if (typeName.compare("int") == 0) {
//     		rewriter.ReplaceText(var->getLocStart(), 3, "double");
//     	}
    	
    	
    	return true;
    }

    // virtual bool VisitStmt(Stmt *st) {
    //     if (ReturnStmt *ret = dyn_cast<ReturnStmt>(st)) {
    //         rewriter.ReplaceText(ret->getRetValue()->getLocStart(), 6, "val");
    //         errs() << "** Rewrote ReturnStmt\n";
    //     }        
    //     if (CallExpr *call = dyn_cast<CallExpr>(st)) {
    //         rewriter.ReplaceText(call->getLocStart(), 7, "add5");
    //         errs() << "** Rewrote function call\n";
    //     } 
    //     // if (DeclStmt *decl = dyn_cast<DeclStmt>(st)) {
    //     // 	rewriter.ReplaceText(decl->getLocStart(), 3, "double");
    //     // }
    //     return true;
    // }
};



class ExampleASTConsumer : public ASTConsumer {
private:
    ExampleVisitor *visitor; // doesn't have to be private

public:
    // override the constructor in order to pass CI
    explicit ExampleASTConsumer(CompilerInstance *CI): visitor(new ExampleVisitor(CI)) {}// initialize the visitor

    // override this to call our ExampleVisitor on the entire source file
    virtual void HandleTranslationUnit(ASTContext &Context) {
        /* we can use ASTContext to get the TranslationUnitDecl, which is
             a single Decl that collectively represents the entire source file */
        visitor->TraverseDecl(Context.getTranslationUnitDecl());
    }
};



class ExampleFrontendAction : public ASTFrontendAction {
public:
    ExampleFrontendAction() {}

    //Note that unique pointer is used.
    virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) {
        CI.getDiagnostics().setClient(new IgnoringDiagConsumer());
        return  llvm::make_unique<ExampleASTConsumer>(&CI); // pass CI pointer to ASTConsumer
    }
};



int main(int argc, const char **argv) {
    // parse json file
    std::ifstream file;
    file.open("config.json");
    
    json j = json::parse(file);
    file.close();
    
    for (json i : j["config"]) {
        json temp = i["localVar"];
        if (temp != nullptr) {
            varToChange vartochange(temp["function"], temp["name"], temp["type"]);
            varList.push_back(vartochange);
        }
    }
    
    
    // parse the command-line args passed to your code
    CommonOptionsParser op(argc, argv, StatSampleCategory);        
    // create a new Clang Tool instance (a LibTooling environment)
    ClangTool Tool(op.getCompilations(), op.getSourcePathList());

    // run the Clang Tool, creating a new FrontendAction (explained below)
    int result = Tool.run(newFrontendActionFactory<ExampleFrontendAction>().get());

    //errs() << "\nFound " << numFunctions << " functions.\n\n";
    // print out the rewritten source code ("rewriter" is a global var.)
    cout << endl;
    cout << endl;
    cout << "This is the new version of code:" << endl;
    cout << endl;
    
    rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(errs());

   
    return result;
}
