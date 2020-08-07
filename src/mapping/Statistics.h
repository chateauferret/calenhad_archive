//
// Created by martin on 01/11/17.
//

#ifndef CALENHAD_STATISTICS_H
#define CALENHAD_STATISTICS_H

namespace calenhad {
    namespace mapping {

        class Statistics {
        public:
            Statistics (double min, double max, double sum, int count, int renderTime, int size);
            double _min, _max, _sum;
            int _count;
            int _renderTime, _size;
            double range() const;
            double mean() const;
            bool ok() const;
        };
    }
}



#endif //CALENHAD_STATISTICS_H
