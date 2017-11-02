//
// Created by martin on 01/11/17.
//

#ifndef CALENHAD_STATISTICS_H
#define CALENHAD_STATISTICS_H

namespace calenhad {
    namespace mapping {

        class Statistics {
        public:
            Statistics (const double& min, const double& max, const double& sum, const int& count);
            double _min, _max, _sum;
            int _count;
            double range();
            double mean();
            bool ok();
        };
    }
}



#endif //CALENHAD_STATISTICS_H
