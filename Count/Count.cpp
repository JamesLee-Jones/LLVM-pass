#include <map>
#include <iterator>
#include <iostream>
#include <fstream>

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
			std::map<int,int> instr_in_block;

			for(auto &F : M) {
				Function *fp = &F;
				if(!fp->isDeclaration()) {
					num_functions++;
					for(auto &B : F) {
						num_basic_blocks++;
						int num_instr = 0;
						for(auto &I : B)
							num_instr++;
						instr_in_block[num_instr]++;
					}
				}
			}

			errs() << "The program has a total of " << num_functions <<
				" functions and " << num_basic_blocks << " basic blocks.\n";

			for(auto const& [key, value] : instr_in_block)
				errs() << key << ": " << value << "\n";

			std::ofstream data_file;
			data_file.open("instructions_per_block.dat");
			data_file << "X Y\n";
			std::map<int,int>::iterator it = instr_in_block.begin();
			while(it != instr_in_block.end()) {
				data_file << it->first << " " << it->second << "\n";
				it++;
			}
			data_file.close();

			std::ofstream file;
			file.open("plot.gp");
			file << "set terminal pdf\n"
			     << "set output \"freqOfInstructionsPerBlock.pdf\"\n"
			     << "set style data histogram\n"
			     << "set style fill solid\n"
			     << "set xrange [0:*]\n"
			     << "set yrange [0:*]\n"
			     << "set term pdfcairo font \"Arial\"\n"
			     << "plot \"instructions_per_block.dat\" using 2 title \"Frequency\"";
			file.close();
			system("gnuplot -p plot.gp");

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
