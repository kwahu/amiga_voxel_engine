#include "common/mod.h"
#include <cstdlib>
#include <vector>
#include <memory>
#include <algorithm>
#include <fstream>
#include <fmt/format.h>

// mod_tool -i ../../../../res/germz1.mod -i ../../../../res/germz2.mod -sp ../../../../build/data/samples.samplepack -o ../../../../build/data/germz1.mod -o ../../../../build/data/germz2.mod
int main(int lArgCount, const char *pArgs[])
{
	if(lArgCount <= 1) {
		fmt::print("ERR: no parameters specified\n");
		return EXIT_FAILURE;
	}

	std::vector<std::shared_ptr<tMod>> vModsIn;
	std::vector<std::string> vOutNames;
	std::string szSamplePackPath;
	bool isStripSamples = true;

	for(int i = 0; i < lArgCount; ++i) {
		if(pArgs[i] == std::string("-i")) {
			vModsIn.push_back(std::make_shared<tMod>(pArgs[++i]));
		}
		else if(pArgs[i] == std::string("-o")) {
			vOutNames.push_back(pArgs[++i]);
		}
		else if(pArgs[i] == std::string("-sp")) {
			szSamplePackPath = pArgs[++i];
		}
	}

	if(vModsIn.size() != vOutNames.size()) {
		fmt::print(
			"ERR: Output path count ({}) doesn't match input file count ({})\n",
			vOutNames.size(), vModsIn.size()
		);
		return EXIT_FAILURE;
	}

	// Get the collection of unique samples
	std::vector<tSample> vMergedSamples;
	for(const auto &pMod: vModsIn) {
		const auto &ModSamples = pMod->getSamples();
		for(const auto &ModSample: ModSamples) {
			// Check if sample is on the list
			auto FoundMergedSample = std::find_if(
				vMergedSamples.begin(), vMergedSamples.end(),
				[&ModSample](const tSample &RefSample) -> bool {
					bool isSameName = (RefSample.m_szName == ModSample.m_szName);
					return isSameName;
				}
			);
			if(FoundMergedSample == vMergedSamples.end()) {
				// Sample not on the list - add it
				vMergedSamples.push_back(ModSample);
			}
			else {
				// Verify that Samples are exactly the same
				if(ModSample != *FoundMergedSample) {
					fmt::print(
						"ERR: sample {} '{}' have different contents in mod '{}' than in others\n",
						(&ModSample - ModSamples.data()), ModSample.m_szName, pMod->getSongName()
					);
					return EXIT_FAILURE;
				}
			}
		}
	}

	// Change the sample indices in modules so that they can be used with the samplepack
	for(const auto &pMod: vModsIn) {
		// Scan the samples to get the reorder table
		std::vector<uint8_t> vReorder; // [idxOld] = idxNew
		for(const auto ModSample: pMod->getSamples()) {
			uint8_t ubIdxNew = 0;
			if(!ModSample.m_szName.empty()) {
				// Find the sample index in new samplepack
				auto SampleNew = std::find_if(
					vMergedSamples.begin(), vMergedSamples.end(),
					[&ModSample](const tSample &MergedSample) -> bool {
						bool isSame = MergedSample.m_szName == ModSample.m_szName;
						return isSame;
					}
				);
				ubIdxNew = std::distance(vMergedSamples.begin(), SampleNew);
			}
			vReorder.push_back(ubIdxNew);
		}

		// Reorder samples, replace sample indices in notes
		pMod->reorderSamples(vReorder);

		// Trim sample data - Don't do it or sample defs will have data length 0
		// pMod->clearSampleData();
	}

	if(!szSamplePackPath.empty()) {
		fmt::print("Writing sample pack to {}...\n", szSamplePackPath);
		std::ofstream FileSamplePack;
		FileSamplePack.open(szSamplePackPath, std::ios::binary);
		for(const auto &Sample: vMergedSamples) {
			FileSamplePack.write(
				reinterpret_cast<const char*>(Sample.m_vData.data()),
				Sample.m_vData.size() * sizeof(Sample.m_vData[0])
			);
		}
	}

	uint32_t ulIdx = 0;
	for(const auto &pMod: vModsIn) {
		fmt::print("Writing {} to {}...\n", pMod->getSongName(), vOutNames[ulIdx]);
		pMod->toMod(vOutNames[ulIdx], isStripSamples);
		++ulIdx;
	}

	fmt::print("All done!\n");
	return EXIT_SUCCESS;
}
