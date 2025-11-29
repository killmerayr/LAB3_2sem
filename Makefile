CXX = /opt/homebrew/opt/llvm/bin/clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O0 -g -fprofile-instr-generate -fcoverage-mapping

# --------------------
# Источники проекта
# --------------------
SRC := $(shell find sd -type f -name "*.cpp")

# --------------------
# Google Test
# --------------------
GTEST_TESTS := $(shell find tests/gtest -type f -name "*.cpp")
GTEST_INCLUDE := -I./sd -I/opt/homebrew/Cellar/googletest/1.17.0/include
GTEST_LIBS := /opt/homebrew/Cellar/googletest/1.17.0/lib/libgtest.a \
              /opt/homebrew/Cellar/googletest/1.17.0/lib/libgtest_main.a -lpthread

# --------------------
# Boost Test
# --------------------
BOOST_TESTS := $(shell find tests/boost -type f -name "*.cpp")
BOOST_INCLUDE := -I./sd -I/opt/homebrew/Cellar/boost/1.89.0/include
BOOST_LIB := -L/opt/homebrew/Cellar/boost/1.89.0/lib -lboost_unit_test_framework

# --------------------
# Catch2 Test
# --------------------
CATCH_INCLUDE := -I./sd -I/opt/homebrew/Cellar/catch2/3.11.0/include
CATCH_MAIN := tests/catch2/catch_main.cpp
CATCH_TESTS := $(filter-out $(CATCH_MAIN), $(shell find tests/catch2 -type f -name "*.cpp"))
CATCH_SRCS := $(shell find sd -type f -name "*.cpp")

.PHONY: all lint build run coverage boost boost_coverage catch catch_coverage clean

# --------------------
# Основные цели
# --------------------
all:
	@echo "Цели: lint, build, run, coverage, boost, boost_coverage, catch, catch_coverage"

# --------------------
# Линтинг
# --------------------
lint:
	@echo "🔍 Запуск clang-tidy..."
	@rm -f lint.log
	@for file in $(SRC) $(GTEST_TESTS) $(BOOST_TESTS) $(CATCH_TESTS) $(CATCH_MAIN); do \
		echo "→ Проверка $$file"; \
		clang-tidy "$$file" -- -std=c++17 -I. >> lint.log 2>&1 || true; \
	done
	@echo "✅ Линтинг завершён. Просмотреть: cat lint.log"

# --------------------
# Google Test
# --------------------
build:
	$(CXX) $(CXXFLAGS) $(GTEST_INCLUDE) $(SRC) $(GTEST_TESTS) -o runGTests $(GTEST_LIBS)

run:
	LLVM_PROFILE_FILE=$(PWD)/coverage.profraw ./runGTests

coverage: build run
	llvm-profdata merge -sparse $(PWD)/coverage.profraw -o coverage.profdata
	llvm-cov show ./runGTests \
		-instr-profile=coverage.profdata \
		-format=html \
		-output-dir=coverage_report \
		-Xdemangler=c++filt
	@echo "Открыть отчёт: open coverage_report/index.html"

# --------------------
# Boost Test
# --------------------
boost:
	@echo "🧪 Компиляция Boost тестов..."
	$(CXX) $(CXXFLAGS) $(BOOST_INCLUDE) $(SRC) $(BOOST_TESTS) -o runBoostTests $(BOOST_LIB)
	@echo "🚀 Запуск Boost тестов..."
	./runBoostTests

boost_coverage:
	@echo "🧪 Компиляция и запуск Boost тестов с покрытием..."
	$(CXX) $(CXXFLAGS) $(BOOST_INCLUDE) $(SRC) $(BOOST_TESTS) -o runBoostTests $(BOOST_LIB)
	LLVM_PROFILE_FILE=$(PWD)/coverage_boost.profraw ./runBoostTests
	@echo "📝 Генерация отчёта покрытия..."
	llvm-profdata merge -sparse $(PWD)/coverage_boost.profraw -o coverage_boost.profdata
	llvm-cov show ./runBoostTests \
		-instr-profile=coverage_boost.profdata \
		-format=html \
		-output-dir=coverage_boost_report \
		-Xdemangler=c++filt
	@echo "✅ Boost coverage готов. Открыть: open coverage_boost_report/index.html"

# --------------------
# Catch2 Test
# --------------------

catch:
	@echo "🧪 Компиляция Catch2 тестов..."
	$(CXX) $(CXXFLAGS) $(CATCH_INCLUDE) $(CATCH_MAIN) $(CATCH_TESTS) $(CATCH_SRCS) \
		-L/opt/homebrew/Cellar/catch2/3.11.0/lib -lCatch2Main -lCatch2 \
		-o runCatchTests
	@echo "🚀 Запуск Catch2 тестов..."
	./runCatchTests

catch_coverage:
	@echo "🧪 Компиляция и запуск Catch2 тестов с покрытием..."
	LLVM_PROFILE_FILE=$(PWD)/coverage_catch.profraw $(CXX) $(CXXFLAGS) $(CATCH_INCLUDE) $(CATCH_MAIN) $(CATCH_TESTS) $(CATCH_SRCS) \
		-L/opt/homebrew/Cellar/catch2/3.11.0/lib -lCatch2Main -lCatch2 \
		-fprofile-instr-generate -fcoverage-mapping \
		-o runCatchTests
	-LLVM_PROFILE_FILE=$(PWD)/coverage_catch.profraw ./runCatchTests
	@echo "📝 Генерация отчёта покрытия..."
	llvm-profdata merge -sparse coverage_catch.profraw -o coverage_catch.profdata
	llvm-cov show ./runCatchTests \
		-instr-profile=coverage_catch.profdata \
		-format=html \
		-output-dir=coverage_catch_report \
		-Xdemangler=c++filt
	@echo "✅ Catch2 coverage готов. Открыть: open coverage_catch_report/index.html"

# --------------------
# Очистка
# --------------------
clean:
	rm -f runGTests runBoostTests runCatchTests \
	      coverage.profraw coverage.profdata \
	      coverage_boost.profraw coverage_boost.profdata \
	      coverage_catch.profraw coverage_catch.profdata
	rm -rf coverage_report coverage_boost_report coverage_catch_report
	rm -f lint.log