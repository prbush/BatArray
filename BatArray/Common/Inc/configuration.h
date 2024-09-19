/*
 * configuration.h
 *
 *  Created on: Jul 29, 2024
 *      Author: philbush
 */

#ifndef INC_CONFIGURATION_H_
#define INC_CONFIGURATION_H_

//#define TESTING

#ifdef TESTING

#define ADC_SAMPLING_RATE (250000U)

#define NUMBER_OF_FILES (1U)

#define MINS_PER_FILE (30U)

#else

#define ADC_SAMPLING_RATE (250000U)

#define NUMBER_OF_FILES (12U)

#define MINS_PER_FILE (60U)

#endif

#endif /* INC_CONFIGURATION_H_ */
