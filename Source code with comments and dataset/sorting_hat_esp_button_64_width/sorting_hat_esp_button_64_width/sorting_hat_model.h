#pragma once
#include <cstdarg>
namespace Eloquent {
    namespace ML {
        namespace Port {
            class DecisionTree {
                public:
                    /**
                    * Predict class for features vector
                    */
                    int predict(float *x) {
                        if (x[5] <= 1.5) {
                            if (x[9] <= 3.5) {
                                return 1;
                            }

                            else {
                                return 3;
                            }
                        }

                        else {
                            if (x[7] <= 1.5) {
                                if (x[8] <= 3.0) {
                                    return 0;
                                }

                                else {
                                    return 3;
                                }
                            }

                            else {
                                if (x[1] <= 3.5) {
                                    if (x[8] <= 2.5) {
                                        if (x[1] <= 2.5) {
                                            return 1;
                                        }

                                        else {
                                            return 2;
                                        }
                                    }

                                    else {
                                        if (x[6] <= 2.5) {
                                            return 0;
                                        }

                                        else {
                                            return 0;
                                        }
                                    }
                                }

                                else {
                                    if (x[7] <= 3.5) {
                                        return 3;
                                    }

                                    else {
                                        return 2;
                                    }
                                }
                            }
                        }
                    }

                    /**
                    * Predict readable class name
                    */
                    const char* predictLabel(float *x) {
                        return idxToLabel(predict(x));
                    }

                    /**
                    * Convert class idx to readable name
                    */
                    const char* idxToLabel(uint8_t classIdx) {
                        switch (classIdx) {
                            case 0:
                            return "Gryffindor";
                            case 1:
                            return "Ravenclaw";
                            case 2:
                            return "Hufflepuff";
                            case 3:
                            return "Slytherin";
                            default:
                            return "Houston we have a problem";
                        }
                    }

                protected:
                };
            }
        }
    }