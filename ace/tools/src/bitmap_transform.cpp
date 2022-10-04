/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "common/bitmap.h"
#include "common/logging.h"
#include "common/parse.h"

class tOp {
public:
	virtual tChunkyBitmap execute(const tChunkyBitmap &Src) = 0;
	virtual std::string toString(void) = 0;
};

class tOpExtract: public tOp {
public:
	tOpExtract(uint16_t uwX, uint16_t uwY, uint16_t uwW, uint16_t uwH);
	virtual tChunkyBitmap execute(const tChunkyBitmap &Src);
	virtual std::string toString(void);
private:
	uint16_t m_uwX, m_uwY, m_uwW, m_uwH;
};

class tOpRotate: public tOp {
public:
	tOpRotate(double dDeg, tRgb Bg);
	virtual tChunkyBitmap execute(const tChunkyBitmap &Src);
	virtual std::string toString(void);
private:
	double m_dDeg;
	tRgb m_Bg;
};

class tOpMirror: public tOp {
public:
	enum class tAxis {
		X, Y
	};

	tOpMirror(tAxis eAxis);
	virtual tChunkyBitmap execute(const tChunkyBitmap &Src);
	virtual std::string toString(void);
private:
	tAxis m_eAxis;
};

void printUsage(const std::string &szAppName)
{
	using fmt::print;
	print("Usage:\n\t{} in.png out.png [transforms]\n", szAppName);
	print("\nAvailable transforms, done in passed order:\n");
	print("\t-extract x y width height\tExtract rectangle at x,y of size width*height.\n");
	print("\t-rotate deg bgcolor      \tRotate clockwise by given number of degrees. Can be negative.\n");
	print("\t-mirror x|y              \tMirror image along x or y axis.\n");
	print("\nCurrently only PNG is supported, sorry!\n");
}

int main(int lArgCount, char *pArgs[])
{
	if(lArgCount - 1 < 2) {
		nLog::error(
			"Invalid number of arguments, got {}, expected at least 2", lArgCount - 1
		);
		printUsage(pArgs[0]);
		return EXIT_FAILURE;
	}

	std::string szSrcPath = pArgs[1];
	auto Img = tChunkyBitmap::fromPng(szSrcPath);
	if(!Img.m_uwHeight) {
		nLog::error("Couldn't open image '{}'", szSrcPath);
		return EXIT_FAILURE;
	}

	std::vector<std::unique_ptr<tOp>> vOps;

	for(auto ArgIdx = 3; ArgIdx < lArgCount; ++ArgIdx) {
		const std::string_view szOp = pArgs[ArgIdx];
		if(szOp == "-extract") {
			if(ArgIdx + 4 >= lArgCount) {
				nLog::error(
					"Too few args for {} - first arg at pos {}, arg count: {}",
					szOp, ArgIdx + 1, lArgCount
				);
				return EXIT_FAILURE;
			}
			auto ArgStart = ArgIdx + 1;
			try {
				auto X = std::stoul(pArgs[++ArgIdx]);
				auto Y = std::stoul(pArgs[++ArgIdx]);
				auto W = std::stoul(pArgs[++ArgIdx]);
				auto H = std::stoul(pArgs[++ArgIdx]);
				vOps.push_back(std::make_unique<tOpExtract>(X, Y, W, H));
			}
			catch(std::exception Ex) {
				nLog::error(
					"Couldn't parse extract args: x:'{}' y:'{}' w:'{}' h:'{}'",
					pArgs[ArgStart + 0], pArgs[ArgStart + 1],
					pArgs[ArgStart + 2], pArgs[ArgStart + 3]
				);
				return EXIT_FAILURE;
			}
		}
		else if(szOp == "-rotate") {
			if(ArgIdx + 2 >= lArgCount) {
				nLog::error(
					"Too few args for {} - first arg at pos {}, arg count: {}",
					szOp, ArgIdx + 1, lArgCount
				);
				return EXIT_FAILURE;
			}
			try {
				auto Deg = std::stod(pArgs[++ArgIdx]);
				auto Bg = tRgb(pArgs[++ArgIdx]);
				vOps.push_back(std::make_unique<tOpRotate>(Deg, Bg));
			}
			catch(std::exception Ex) {
				nLog::error("Couldn't parse rotate degrees: '{}'", pArgs[ArgIdx]);
				return EXIT_FAILURE;
			}
		}
		else if(szOp == "-mirror") {
			if(ArgIdx + 1 >= lArgCount) {
				nLog::error("Too few args for {}", szOp);
				return EXIT_FAILURE;
			}
			std::string_view szAxis = pArgs[++ArgIdx];
			if(szAxis != "x" && szAxis != "y") {
				nLog::error("Unknown mirror axis specified: '{}'", szAxis);
				return EXIT_FAILURE;
			}
			vOps.push_back(std::make_unique<tOpMirror>(
				szAxis == "x" ? tOpMirror::tAxis::X : tOpMirror::tAxis::Y
			));
		}
		else {
			nLog::error("Unknown argument: '{}'", szOp);
			return EXIT_FAILURE;
		}
	}

	for(const auto &Op: vOps) {
		Img = Op->execute(Img);
		if(!Img.m_uwWidth) {
			nLog::error("Operation {} failed!", Op->toString());
			return EXIT_FAILURE;
		}
	}

	std::string szDstPath = pArgs[2];
	bool isOk = Img.toPng(szDstPath);
	if(!isOk) {
		nLog::error("Couldn't write to '{}'", szDstPath);
		return EXIT_FAILURE;
	}

	fmt::print("All done!\n");
	return EXIT_SUCCESS;
}

//--------------------------------------------------------------------OP EXTRACT

tOpExtract::tOpExtract(uint16_t uwX, uint16_t uwY, uint16_t uwW, uint16_t uwH):
	m_uwX(uwX), m_uwY(uwY), m_uwW(uwW), m_uwH(uwH)
{
	// Nothing here for now
}

tChunkyBitmap tOpExtract::execute(const tChunkyBitmap &Source)
{
	fmt::print(
		"Extracting rectangle {}x{} at {},{}\n", m_uwW, m_uwH, m_uwX, m_uwY
	);
	tChunkyBitmap Dst(m_uwW, m_uwH);
	bool isOk = Source.copyRect(m_uwX, m_uwY, Dst, 0, 0, m_uwW, m_uwH);
	if(!isOk) {
		nLog::error("Couldn't extract given rectangle from source image");
		Dst = tChunkyBitmap();
	}
	return Dst;
}

std::string tOpExtract::toString(void)
{
	return fmt::format(
		"extract(x: {}, y: {}, w: {}, h: {})", m_uwX, m_uwY, m_uwW, m_uwH
	);
}

//---------------------------------------------------------------------OP MIRROR

tOpMirror::tOpMirror(tOpMirror::tAxis eAxis):
	m_eAxis(eAxis)
{
	// Nothing here for now
}

tChunkyBitmap tOpMirror::execute(const tChunkyBitmap &Source)
{
	tChunkyBitmap Dst(Source.m_uwWidth, Source.m_uwHeight);
	if(m_eAxis == tAxis::X) {
		for(auto X = 0; X < Source.m_uwWidth; ++X) {
			for(auto Y = 0; Y < Source.m_uwHeight; ++Y) {
				Dst.pixelAt(X, Y) = Source.pixelAt(X, (Source.m_uwHeight - 1) - Y);
			}
		}
	}
	else {
		for(auto X = 0; X < Source.m_uwWidth; ++X) {
			for(auto Y = 0; Y < Source.m_uwHeight; ++Y) {
				Dst.pixelAt(X, Y) = Source.pixelAt((Source.m_uwWidth - 1) - X, Y);
			}
		}
	}
	return Dst;
}

std::string tOpMirror::toString(void)
{
	return fmt::format("mirror(axis: {})", m_eAxis == tAxis::X ? 'X' : 'Y');
}

//---------------------------------------------------------------------OP ROTATE

tOpRotate::tOpRotate(double dDeg, tRgb Bg):
	m_dDeg(dDeg), m_Bg(Bg)
{
	// Nothing here for now
}

tChunkyBitmap tOpRotate::execute(const tChunkyBitmap &Source) {
	tChunkyBitmap Dst(Source.m_uwWidth, Source.m_uwHeight);
	auto CenterX = (Source.m_uwWidth - 1) / 2.0;
	auto CenterY = (Source.m_uwHeight - 1) / 2.0;

	auto Rad = (m_dDeg * 2 * M_PI) / 360;
	auto CalcCos = cos(Rad);
	auto CalcSin = sin(Rad);

	// For each of new bitmap's pixel sample color from rotated source x,y
	for(auto Y = 0; Y < Dst.m_uwHeight; ++Y) {
		auto Dy = Y - CenterY;
		for(auto X = 0; X < Dst.m_uwWidth; ++X) {
			auto Dx = X - CenterX;
			auto U = round(CalcCos * Dx + CalcSin * Dy + (CenterX));
			auto V = round(-CalcSin * Dx + CalcCos * Dy + (CenterY));

			if(U < 0 || V < 0 || U >= Dst.m_uwWidth || V >= Dst.m_uwHeight) {
				// fmt::print("can't sample for {:2d},{:2d} from {:.1f},{:.1f}\n", X, Y, U, V);
				Dst.pixelAt(X, Y) = m_Bg;
			}
			else {
				Dst.pixelAt(X, Y) = Source.pixelAt(U, V);
			}
		}
	}

	return Dst;
}

std::string tOpRotate::toString(void)
{
	return fmt::format("rotate(deg: {}, bg: {})", m_dDeg, m_Bg.toString());
}
