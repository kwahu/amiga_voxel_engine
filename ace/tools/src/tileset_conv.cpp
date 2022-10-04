/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "common/bitmap.h"
#include "common/logging.h"
#include "common/parse.h"
#include "common/fs.h"

void printUsage(const std::string &szAppName)
{
	// tileset_conv inPath tileSize outPath
	using fmt::print;
	print("Usage:\n\t{} inPath tileSize outPath\n\n", szAppName);
	print("inPath  \t- path to input file/directory\n");
	print("tileSize\t- size of tile's edge, in pixels\n");
	print("outPath \t- path to output file/directory\n");
	print("\nWhen using .bm as output:\n");
	print("-i              \t- save as interleaved bitmap\n");
	print("\nWhen using .bm as input or output:\n");
	print("-plt palettePath\t- use following palette\n");
	print("\nAdditional options:\n");
	print("-cols        \t- number of columns to use in output (default: 1)\n");
	print("-h tileHeight\t- override height for rectangular tiles\n");
}

int main(int lArgCount, const char *pArgs[])
{
	if(lArgCount - 1 < 3) {
		nLog::error("Too few arguments, got {}", lArgCount - 1);
		printUsage(pArgs[0]);
		return EXIT_FAILURE;
	}

	int32_t lTileSize;
	if(!nParse::toInt32(pArgs[2], "tileSize", lTileSize) || lTileSize < 0) {
		return EXIT_FAILURE;
	}
	int32_t lTileHeight = lTileSize;

	std::string szInPath = pArgs[1];
	std::string szOutPath = pArgs[3];
	bool isInterleaved = false;
	tPalette Palette;
	int32_t lColumns = 1;
	for(uint8_t i = 4; i < lArgCount - 1; ++i) {
		if(pArgs[i] == std::string("-i")) {
			isInterleaved = true;
		}
		else if(pArgs[i] == std::string("-plt") && i + 1 <= lArgCount - 1) {
			++i;
			Palette = tPalette::fromFile(pArgs[i]);
			if(!Palette.m_vColors.size()) {
				nLog::error("Couldn't read palette: '{}'", pArgs[i]);
				return EXIT_FAILURE;
			}
			fmt::print(
				"Read {} colors from '{}'\n", Palette.m_vColors.size(), pArgs[i]
			);
		}
		else if(pArgs[i] == std::string("-cols") && i + 1 <= lArgCount - 1) {
			++i;
			if(!nParse::toInt32(pArgs[i], "-cols", lColumns) || lColumns <= 0) {
				return EXIT_FAILURE;
			}
		}
		else if(pArgs[i] == std::string("-h") && i + 1 <= lArgCount - 1) {
			++i;
			if(!nParse::toInt32(pArgs[i], "-h", lTileHeight) || lTileHeight <= 0) {
				return EXIT_FAILURE;
			}
			fmt::print("Override tile height to {}\n", lTileHeight);
		}
	}

	// Read input
	std::string szInExt = nFs::getExt(szInPath);
	std::vector<tChunkyBitmap> vTiles; // NOTE: empty tiles will have zero w/h
	if(szInExt == "png" || szInExt == "bm") {
		tChunkyBitmap In;
		if(szInExt == "png") {
			In = tChunkyBitmap::fromPng(szInPath);
		}
		else if(szInExt == "bm") {
			auto InPlanar = tPlanarBitmap::fromBm(szInPath);
			In = tChunkyBitmap(InPlanar, Palette);
		}
		if(In.m_uwHeight <= 0) {
			nLog::error("Couldn't load input file: '{}'", szInPath);
			return EXIT_FAILURE;
		}
		if(In.m_uwHeight % lTileHeight != 0) {
			nLog::error("Input bitmap height is not divisible by {}", lTileHeight);
			return EXIT_FAILURE;
		}
		if(In.m_uwWidth % lTileSize != 0) {
			nLog::error("Input bitmap width is not divisible by {}", lTileSize);
			return EXIT_FAILURE;
		}
		uint16_t uwTileCountHoriz = In.m_uwWidth / lTileSize;
		uint16_t uwTileCountVert = In.m_uwHeight / lTileHeight;
		vTiles.reserve(uwTileCountHoriz * uwTileCountVert);
		for(uint16_t y = 0; y < uwTileCountVert; ++y) {
			for(uint16_t x = 0; x < uwTileCountHoriz; ++x) {
				tChunkyBitmap Tile(lTileSize, lTileHeight);
				In.copyRect(x * lTileSize, y * lTileHeight, Tile, 0, 0, lTileSize, lTileHeight);
				vTiles.push_back(std::move(Tile));
			}
		}
	}
	else if(nFs::isDir(szInPath)) {
		int16_t wLastFull = -1;
		for(uint16_t i = 0; i < 256; ++i) {
			auto Tile = tChunkyBitmap::fromPng(fmt::format("{}/{}.png", szInPath, i));
			if(Tile.m_uwHeight != 0) {
				wLastFull = i;
			}
			vTiles.push_back(std::move(Tile));
		}
		vTiles.resize(wLastFull + 1);
	}
	else {
		nLog::error("Unsupported input extension: '{}'", szInExt);
		return EXIT_FAILURE;
	}

	// Save output
	uint16_t uwTileCount = vTiles.size();
	fmt::print("Read {} tiles from '{}'\n", uwTileCount, szInPath);
	std::string szOutExt = nFs::getExt(szOutPath);
	if(szOutExt == "png" || szOutExt == "bm") {
		tRgb Bg;
		if(Palette.m_vColors.size()) {
			Bg = Palette.m_vColors.at(0);
		}
		fmt::print("Using color for bg: #{:02X}{:02X}{:02X}\n", Bg.ubR, Bg.ubG, Bg.ubB);
		tChunkyBitmap Out(lTileSize * lColumns, (uwTileCount / lColumns + (uwTileCount % lColumns != 0)) * lTileHeight, Bg);
		for(uint16_t i = 0; i < uwTileCount; ++i) {
			auto &Tile = vTiles.at(i);
			if(Tile.m_uwHeight != 0) {
				Tile.copyRect(0, 0, Out, (i % lColumns) * lTileSize, lTileHeight * (i / lColumns), lTileSize, lTileHeight);
			}
		}
		if(szOutExt == "png" && !Out.toPng(szOutPath)) {
			nLog::error("Couldn't write output to '{}'", szOutPath);
			return EXIT_FAILURE;
		}
		else if(szOutExt == "bm") {
			if(Palette.m_vColors.size() == 0) {
				nLog::error("No palette specified");
				return EXIT_FAILURE;
			}
			tPlanarBitmap PlanarOut(Out, Palette);
			if(PlanarOut.m_uwHeight == 0) {
				nLog::error("Problem with planar conversion");
				return EXIT_FAILURE;
			}
			if(!PlanarOut.toBm(szOutPath, isInterleaved)) {
				nLog::error("Couldn't write to '{}'", szOutPath);
				return EXIT_FAILURE;
			}
		}
	}
	else if(szOutExt == "") {
		// Tile directory
		nFs::dirCreate(szOutPath);
		for(uint16_t i = 0; i < uwTileCount; ++i) {
			auto &Tile = vTiles.at(i);
			if(Tile.m_uwHeight != 0) {
				std::string szTilePath = fmt::format("{}/{}.png", szOutPath, i);
				if(!Tile.toPng(szTilePath)) {
					nLog::error("Couldn't write tile to '{}'", szTilePath);
					return EXIT_FAILURE;
				}
			}
		}
	}
	else {
		nLog::error("Unsupported output extension: '{}'", szOutExt);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
