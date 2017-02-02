
#include <gtest/gtest.h>
#include "../libnoiseutils/intervallegend.h"

using namespace noise::utils;

class IntervalLegendTest : public ::testing::Test {
    protected:
    IntervalLegend legend = IntervalLegend();
    QColor colours [8] = {
        QColor (0, 0, 0), QColor (0, 0, 255), QColor (0, 255, 0), QColor (0, 255, 255),
        QColor (255, 0, 0), QColor (255, 0, 255), QColor (255, 255, 0), QColor (255, 255, 255)
    };

    virtual void SetUp() {
        for (int i = 0; i < 8; i++) {
            legend.addEntry (100 * i, colours [i]);
        }
    }

    virtual void TearDown() {
    }
};

TEST_F (IntervalLegendTest, findValue) {
   EXPECT_EQ (colours [0], legend.lookup (50));
   EXPECT_EQ (colours [1], legend.lookup (150));
   EXPECT_EQ (colours [2], legend.lookup (250));
}

TEST_F (IntervalLegendTest, insertEntry) {
    QColor newColour = QColor (127, 127, 127);
    legend.addEntry (50, newColour);
    EXPECT_EQ (colours [0], legend.lookup (25));
    EXPECT_EQ (newColour, legend.lookup (75));
    EXPECT_EQ (colours [1], legend.lookup (125));
}

TEST_F (IntervalLegendTest, removeEntry) {
    legend.removeEntries (60, 140);
    EXPECT_EQ (colours [0], legend.lookup (50));
    EXPECT_EQ (colours [0], legend.lookup (50));
    EXPECT_EQ (colours [2], legend.lookup (250));
}

TEST_F (IntervalLegendTest, replaceEntry) {
    QColor newColour = QColor (127, 127, 127);
    legend.addEntry(300, newColour);
    EXPECT_EQ (colours [2], legend.lookup (250));
    EXPECT_EQ (newColour, legend.lookup (301));
    EXPECT_EQ (colours [4], legend.lookup (450));
}

TEST_F (IntervalLegendTest, highestEntry) {
    EXPECT_EQ (colours [7], legend.lookup (10000));
}

TEST_F (IntervalLegendTest, belowMinimum) {
    QColor noValue = QColor (0, 0, 0, 0);
    EXPECT_EQ (noValue, legend.lookup (-1));
}

TEST_F (IntervalLegendTest, entryOnBoundary) {
    EXPECT_EQ (colours [1], legend.lookup (100));
    EXPECT_EQ (colours [1].rgb(), legend.lookup (100).rgb());
}

