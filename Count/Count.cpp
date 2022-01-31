#include "llvm/Pass.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {
	struct Count : public ModulePass {
		static char ID;
		Count() : ModulePass(ID) {}

		virtual bool runOnModule(Module &M) override {
			long num_functions = 0;
			long num_basic_blocks = 0;

			for(auto &F : M) {
				num_functions++;
				errs().write_escaped(F.getName()) << "\n";
				for(auto &B : F)
					num_basic_blocks++;
			}

			errs() << "The program has a total of " << num_functions <<
				" functions and " << num_basic_blocks << " basic blocks.\n";

			return false;
		}
	};
}

char Count::ID = 0;
static RegisterPass<Count> X("count", "Count Pass", false, false);

static llvm::RegisterStandardPasses Y(
		llvm::PassManagerBuilder::EP_EarlyAsPossible,
		[](const llvm::PassManagerBuilder &Builder,
			llvm::legacy::PassManagerBase &PM) {PM.add(new Count());});
