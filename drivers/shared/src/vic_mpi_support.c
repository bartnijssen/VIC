/******************************************************************************
 * @section DESCRIPTION
 *
 * MPI support routines for VIC
 *
 * @section LICENSE
 *
 * The Variable Infiltration Capacity (VIC) macroscale hydrological model
 * Copyright (C) 2014 The Land Surface Hydrology Group, Department of Civil
 * and Environmental Engineering, University of Washington.
 *
 * The VIC model is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *****************************************************************************/

#include <vic_def.h>
#include <vic_mpi.h>


void
initialize_mpi(void)
{
    extern MPI_Datatype mpi_global_struct_type;
    extern MPI_Datatype mpi_option_struct_type;
    extern MPI_Datatype mpi_param_struct_type;
    extern int          mpi_rank;
    extern int          mpi_size;
    int                 status;

    // get MPI mpi_rank and mpi_size
    status = MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    if (status != MPI_SUCCESS) {
        log_err("MPI error in main(): %d\n", status);
    }

    status = MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    if (status != MPI_SUCCESS) {
        log_err("MPI error in main(): %d\n", status);
    }

    // initialize MPI data structures
    create_MPI_global_struct_type(&mpi_global_struct_type);
    create_MPI_option_struct_type(&mpi_option_struct_type);
    create_MPI_param_struct_type(&mpi_param_struct_type);
}

/******************************************************************************
 * @brief   Create an MPI_Datatype that represents the global_param_struct
 * @details This allows MPI operations in which the entire global_param_struct
 *          can be treated as an MPI_Datatype. NOTE: This function needs to be
 *          kept in-sync with the global_param_struct data type in vic_def.h.
 *
 * @param mpi_type MPI_Datatype that can be used in MPI operations
 *****************************************************************************/
void
create_MPI_global_struct_type(MPI_Datatype *mpi_type)
{
    int           nitems; // number of elements in struct
    int           status;
    int          *blocklengths;
    size_t        i;
    MPI_Aint     *offsets;
    MPI_Datatype *mpi_types;

    // nitems has to equal the number of elements in global_param_struct
    nitems = 23;
    blocklengths = (int *) malloc(nitems * sizeof(int));
    if (blocklengths == NULL) {
        log_err("Memory allocation error in create_MPI_global_struct_type().")
    }

    offsets = (MPI_Aint *) malloc(nitems * sizeof(MPI_Aint));
    if (offsets == NULL) {
        log_err("Memory allocation error in create_MPI_global_struct_type().")
    }

    mpi_types = (MPI_Datatype *) malloc(nitems * sizeof(MPI_Datatype));
    if (mpi_types == NULL) {
        log_err("Memory allocation error in create_MPI_global_struct_type().")
    }

    // most of the elements in global_param_struct are not arrays. Use 1 as
    // the default block length and reset as needed
    for (i = 0; i < (size_t) nitems; i++) {
        blocklengths[i] = 1;
    }

    // reset i
    i = 0;

    // double measure_h;
    offsets[i] = offsetof(global_param_struct, measure_h);
    mpi_types[i++] = MPI_DOUBLE;

    // double wind_h;
    offsets[i] = offsetof(global_param_struct, wind_h);
    mpi_types[i++] = MPI_DOUBLE;

    // double resolution;
    offsets[i] = offsetof(global_param_struct, resolution);
    mpi_types[i++] = MPI_DOUBLE;

    // unsigned dt;
    offsets[i] = offsetof(global_param_struct, dt);
    mpi_types[i++] = MPI_UNSIGNED;

    // unsigned out_dt;
    offsets[i] = offsetof(global_param_struct, out_dt);
    mpi_types[i++] = MPI_UNSIGNED;

    // unsigned short endday;
    offsets[i] = offsetof(global_param_struct, endday);
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // unsigned short endmonth;
    offsets[i] = offsetof(global_param_struct, endmonth);
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // unsigned short endyear;
    offsets[i] = offsetof(global_param_struct, endyear);
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // unsigned short forceday[2];
    offsets[i] = offsetof(global_param_struct, forceday);
    blocklengths[i] = 2;
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // unsigned short forcehour[2];
    offsets[i] = offsetof(global_param_struct, forcehour);
    blocklengths[i] = 2;
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // unsigned short forcemonth[2];
    offsets[i] = offsetof(global_param_struct, forcemonth);
    blocklengths[i] = 2;
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // unsigned short forceoffset[2];
    offsets[i] = offsetof(global_param_struct, forceoffset);
    blocklengths[i] = 2;
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // unsigned short forceskip[2];
    offsets[i] = offsetof(global_param_struct, forceskip);
    blocklengths[i] = 2;
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // unsigned short forceyear[2];
    offsets[i] = offsetof(global_param_struct, forceyear);
    blocklengths[i] = 2;
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // unsigned nrecs;
    offsets[i] = offsetof(global_param_struct, nrecs);
    mpi_types[i++] = MPI_UNSIGNED;

    // unsigned short skipyear;
    offsets[i] = offsetof(global_param_struct, skipyear);
    mpi_types[i++] = MPI_DOUBLE;

    // unsigned short startday;
    offsets[i] = offsetof(global_param_struct, startday);
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // unsigned short starthour;
    offsets[i] = offsetof(global_param_struct, starthour);
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // unsigned short startmonth;
    offsets[i] = offsetof(global_param_struct, startmonth);
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // unsigned short startyear;
    offsets[i] = offsetof(global_param_struct, startyear);
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // unsigned short stateday;
    offsets[i] = offsetof(global_param_struct, stateday);
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // unsigned short statemonth;
    offsets[i] = offsetof(global_param_struct, statemonth);
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // unsigned short stateyear;
    offsets[i] = offsetof(global_param_struct, stateyear);
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // make sure that the we have the right number of elements
    if (i != (size_t) nitems) {
        log_err("Miscount in create_MPI_global_struct_type(): "
                "%zd not equal to %d\n", i, nitems);
    }

    status = MPI_Type_create_struct(nitems, blocklengths, offsets, mpi_types,
                                    mpi_type);
    if (status != MPI_SUCCESS) {
        log_err("MPI error in create_MPI_global_struct_type(): %d\n", status);
    }

    status = MPI_Type_commit(mpi_type);
    if (status != MPI_SUCCESS) {
        log_err("MPI error in create_MPI_global_struct_type(): %d\n", status);
    }

    // cleanup
    free(blocklengths);
    free(offsets);
    free(mpi_types);
}

/******************************************************************************
 * @brief   Create an MPI_Datatype that represents the option_struct
 * @details This allows MPI operations in which the entire option_struct can
 *          be treated as an MPI_Datatype. NOTE: This function needs to be kept
 *          in-sync with the option_struct data type in vic_def.h.
 *
 * @param mpi_type MPI_Datatype that can be used in MPI operations
 *****************************************************************************/
void
create_MPI_option_struct_type(MPI_Datatype *mpi_type)
{
    int           nitems; // number of elements in struct
    int           status;
    int          *blocklengths;
    size_t        i;
    MPI_Aint     *offsets;
    MPI_Datatype *mpi_types;

    // nitems has to equal the number of elements in option_struct
    nitems = 69;
    blocklengths = (int *) malloc(nitems * sizeof(int));
    if (blocklengths == NULL) {
        log_err("Memory allocation error in create_MPI_option_struct_type().")
    }

    offsets = (MPI_Aint *) malloc(nitems * sizeof(MPI_Aint));
    if (offsets == NULL) {
        log_err("Memory allocation error in create_MPI_option_struct_type().")
    }

    mpi_types = (MPI_Datatype *) malloc(nitems * sizeof(MPI_Datatype));
    if (mpi_types == NULL) {
        log_err("Memory allocation error in create_MPI_option_struct_type().")
    }

    // none of the elements in option_struct are arrays
    for (i = 0; i < (size_t) nitems; i++) {
        blocklengths[i] = 1;
    }

    // reset i
    i = 0;

    // short AboveTreelineVeg;
    offsets[i] = offsetof(option_struct, AboveTreelineVeg);
    mpi_types[i++] = MPI_SHORT;

    // unsigned short AERO_RESIST_CANSNOW;
    offsets[i] = offsetof(option_struct, AERO_RESIST_CANSNOW);
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // bool BLOWING;
    offsets[i] = offsetof(option_struct, BLOWING);
    mpi_types[i++] = MPI_C_BOOL;

    // bool BLOWING_VAR_THRESHOLD;
    offsets[i] = offsetof(option_struct, BLOWING_VAR_THRESHOLD);
    mpi_types[i++] = MPI_C_BOOL;

    // bool BLOWING_CALC_PROB;
    offsets[i] = offsetof(option_struct, BLOWING_CALC_PROB);
    mpi_types[i++] = MPI_C_BOOL;

    // bool BLOWING_SIMPLE;
    offsets[i] = offsetof(option_struct, BLOWING_SIMPLE);
    mpi_types[i++] = MPI_C_BOOL;

    // bool BLOWING_FETCH;
    offsets[i] = offsetof(option_struct, BLOWING_FETCH);
    mpi_types[i++] = MPI_C_BOOL;

    // bool BLOWING_SPATIAL_WIND;
    offsets[i] = offsetof(option_struct, BLOWING_SPATIAL_WIND);
    mpi_types[i++] = MPI_C_BOOL;

    // bool CARBON;
    offsets[i] = offsetof(option_struct, CARBON);
    mpi_types[i++] = MPI_C_BOOL;

    // bool CLOSE_ENERGY;
    offsets[i] = offsetof(option_struct, CLOSE_ENERGY);
    mpi_types[i++] = MPI_C_BOOL;

    // bool COMPUTE_TREELINE;
    offsets[i] = offsetof(option_struct, COMPUTE_TREELINE);
    mpi_types[i++] = MPI_C_BOOL;

    // bool CONTINUEONERROR;
    offsets[i] = offsetof(option_struct, CONTINUEONERROR);
    mpi_types[i++] = MPI_C_BOOL;

    // bool CORRPREC;
    offsets[i] = offsetof(option_struct, CORRPREC);
    mpi_types[i++] = MPI_C_BOOL;

    // bool EQUAL_AREA;
    offsets[i] = offsetof(option_struct, EQUAL_AREA);
    mpi_types[i++] = MPI_C_BOOL;

    // bool EXP_TRANS;
    offsets[i] = offsetof(option_struct, EXP_TRANS);
    mpi_types[i++] = MPI_C_BOOL;

    // bool FROZEN_SOIL;
    offsets[i] = offsetof(option_struct, FROZEN_SOIL);
    mpi_types[i++] = MPI_C_BOOL;

    // bool FULL_ENERGY;
    offsets[i] = offsetof(option_struct, FULL_ENERGY);
    mpi_types[i++] = MPI_C_BOOL;

    // unsigned short GRND_FLUX_TYPE;
    offsets[i] = offsetof(option_struct, GRND_FLUX_TYPE);
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // bool IMPLICIT;
    offsets[i] = offsetof(option_struct, IMPLICIT);
    mpi_types[i++] = MPI_C_BOOL;

    // bool JULY_TAVG_SUPPLIED;
    offsets[i] = offsetof(option_struct, JULY_TAVG_SUPPLIED);
    mpi_types[i++] = MPI_C_BOOL;

    // bool LAKES;
    offsets[i] = offsetof(option_struct, LAKES);
    mpi_types[i++] = MPI_C_BOOL;

    // unsigned short LW_CLOUD;
    offsets[i] = offsetof(option_struct, LW_CLOUD);
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // unsigned short LW_TYPE;
    offsets[i] = offsetof(option_struct, LW_TYPE);
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // bool MTCLIM_SWE_CORR;
    offsets[i] = offsetof(option_struct, MTCLIM_SWE_CORR);
    mpi_types[i++] = MPI_C_BOOL;

    // size_t Ncanopy;
    offsets[i] = offsetof(option_struct, Ncanopy);
    mpi_types[i++] = MPI_AINT; // note there is no MPI_SIZE_T equivalent

    // size_t Nfrost;
    offsets[i] = offsetof(option_struct, Nfrost);
    mpi_types[i++] = MPI_AINT; // note there is no MPI_SIZE_T equivalent

    // size_t Nlakenode;
    offsets[i] = offsetof(option_struct, Nlakenode);
    mpi_types[i++] = MPI_AINT; // note there is no MPI_SIZE_T equivalent

    // size_t Nlayer;
    offsets[i] = offsetof(option_struct, Nlayer);
    mpi_types[i++] = MPI_AINT; // note there is no MPI_SIZE_T equivalent

    // size_t Nnode;
    offsets[i] = offsetof(option_struct, Nnode);
    mpi_types[i++] = MPI_AINT; // note there is no MPI_SIZE_T equivalent

    // bool NOFLUX;
    offsets[i] = offsetof(option_struct, NOFLUX);
    mpi_types[i++] = MPI_C_BOOL;

    // size_t NVEGTYPES;
    offsets[i] = offsetof(option_struct, NVEGTYPES);
    mpi_types[i++] = MPI_AINT; // note there is no MPI_SIZE_T equivalent

    // bool PLAPSE;
    offsets[i] = offsetof(option_struct, PLAPSE);
    mpi_types[i++] = MPI_C_BOOL;

    // unsigned short RC_MODE;
    offsets[i] = offsetof(option_struct, RC_MODE);
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // size_t ROOT_ZONES;
    offsets[i] = offsetof(option_struct, ROOT_ZONES);
    mpi_types[i++] = MPI_AINT; // note there is no MPI_SIZE_T equivalent

    // bool QUICK_FLUX;
    offsets[i] = offsetof(option_struct, QUICK_FLUX);
    mpi_types[i++] = MPI_C_BOOL;

    // bool QUICK_SOLVE;
    offsets[i] = offsetof(option_struct, QUICK_SOLVE);
    mpi_types[i++] = MPI_C_BOOL;

    // bool SHARE_LAYER_MOIST;
    offsets[i] = offsetof(option_struct, SHARE_LAYER_MOIST);
    mpi_types[i++] = MPI_C_BOOL;

    // unsigned short SNOW_DENSITY;
    offsets[i] = offsetof(option_struct, SNOW_DENSITY);
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // size_t SNOW_BAND;
    offsets[i] = offsetof(option_struct, SNOW_BAND);
    mpi_types[i++] = MPI_AINT; // note there is no MPI_SIZE_T equivalent

    // unsigned SNOW_STEP;
    offsets[i] = offsetof(option_struct, SNOW_STEP);
    mpi_types[i++] = MPI_UNSIGNED;

    // bool SPATIAL_FROST;
    offsets[i] = offsetof(option_struct, SPATIAL_FROST);
    mpi_types[i++] = MPI_C_BOOL;

    // bool SPATIAL_SNOW;
    offsets[i] = offsetof(option_struct, SPATIAL_SNOW);
    mpi_types[i++] = MPI_C_BOOL;

    // bool TFALLBACK;
    offsets[i] = offsetof(option_struct, TFALLBACK);
    mpi_types[i++] = MPI_C_BOOL;

    // bool VP_INTERP;
    offsets[i] = offsetof(option_struct, VP_INTERP);
    mpi_types[i++] = MPI_C_BOOL;

    // unsigned short VP_ITER;
    offsets[i] = offsetof(option_struct, VP_ITER);
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // bool ALMA_INPUT;
    offsets[i] = offsetof(option_struct, ALMA_INPUT);
    mpi_types[i++] = MPI_C_BOOL;

    // bool BASEFLOW;
    offsets[i] = offsetof(option_struct, BASEFLOW);
    mpi_types[i++] = MPI_C_BOOL;

    // unsigned short GRID_DECIMAL;
    offsets[i] = offsetof(option_struct, GRID_DECIMAL);
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // bool VEGLIB_PHOTO;
    offsets[i] = offsetof(option_struct, VEGLIB_PHOTO);
    mpi_types[i++] = MPI_C_BOOL;

    // bool VEGLIB_VEGCOVER;
    offsets[i] = offsetof(option_struct, VEGLIB_VEGCOVER);
    mpi_types[i++] = MPI_C_BOOL;

    // bool VEGPARAM_ALB;
    offsets[i] = offsetof(option_struct, VEGPARAM_ALB);
    mpi_types[i++] = MPI_C_BOOL;

    // bool VEGPARAM_LAI;
    offsets[i] = offsetof(option_struct, VEGPARAM_LAI);
    mpi_types[i++] = MPI_C_BOOL;

    // bool VEGPARAM_VEGCOVER;
    offsets[i] = offsetof(option_struct, VEGPARAM_VEGCOVER);
    mpi_types[i++] = MPI_C_BOOL;

    // unsigned short ALB_SRC;
    offsets[i] = offsetof(option_struct, ALB_SRC);
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // unsigned short LAI_SRC;
    offsets[i] = offsetof(option_struct, LAI_SRC);
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // unsigned short VEGCOVER_SRC;
    offsets[i] = offsetof(option_struct, VEGCOVER_SRC);
    mpi_types[i++] = MPI_UNSIGNED_SHORT;

    // bool LAKE_PROFILE;
    offsets[i] = offsetof(option_struct, LAKE_PROFILE);
    mpi_types[i++] = MPI_C_BOOL;

    // bool ORGANIC_FRACT;
    offsets[i] = offsetof(option_struct, ORGANIC_FRACT);
    mpi_types[i++] = MPI_C_BOOL;

    // bool BINARY_STATE_FILE;
    offsets[i] = offsetof(option_struct, BINARY_STATE_FILE);
    mpi_types[i++] = MPI_C_BOOL;

    // bool INIT_STATE;
    offsets[i] = offsetof(option_struct, INIT_STATE);
    mpi_types[i++] = MPI_C_BOOL;

    // bool SAVE_STATE;
    offsets[i] = offsetof(option_struct, SAVE_STATE);
    mpi_types[i++] = MPI_C_BOOL;

    // bool ALMA_OUTPUT;
    offsets[i] = offsetof(option_struct, ALMA_OUTPUT);
    mpi_types[i++] = MPI_C_BOOL;

    // bool BINARY_OUTPUT;
    offsets[i] = offsetof(option_struct, BINARY_OUTPUT);
    mpi_types[i++] = MPI_C_BOOL;

    // bool COMPRESS;
    offsets[i] = offsetof(option_struct, COMPRESS);
    mpi_types[i++] = MPI_C_BOOL;

    // bool MOISTFRACT;
    offsets[i] = offsetof(option_struct, MOISTFRACT);
    mpi_types[i++] = MPI_C_BOOL;

    // size_t Noutfiles;
    offsets[i] = offsetof(option_struct, Noutfiles);
    mpi_types[i++] = MPI_AINT; // note there is no MPI_SIZE_T equivalent

    // bool OUTPUT_FORCE;
    offsets[i] = offsetof(option_struct, OUTPUT_FORCE);
    mpi_types[i++] = MPI_C_BOOL;

    // bool PRT_HEADER;
    offsets[i] = offsetof(option_struct, PRT_HEADER);
    mpi_types[i++] = MPI_C_BOOL;

    // bool PRT_SNOW_BAND;
    offsets[i] = offsetof(option_struct, PRT_SNOW_BAND);
    mpi_types[i++] = MPI_C_BOOL;

    // make sure that the we have the right number of elements
    if (i != (size_t) nitems) {
        log_err("Miscount in create_MPI_option_struct_type(): "
                "%zd not equal to %d\n", i, nitems);
    }

    status = MPI_Type_create_struct(nitems, blocklengths, offsets, mpi_types,
                                    mpi_type);
    if (status != MPI_SUCCESS) {
        log_err("MPI error in create_MPI_option_struct_type(): %d\n", status);
    }
    status = MPI_Type_commit(mpi_type);
    if (status != MPI_SUCCESS) {
        log_err("MPI error in create_MPI_option_struct_type(): %d\n", status);
    }

    // cleanup
    free(blocklengths);
    free(offsets);
    free(mpi_types);
}

/******************************************************************************
 * @brief   Create an MPI_Datatype that represents the parameters_struct
 * @details This allows MPI operations in which the entire parameters_struct
 *          can be treated as an MPI_Datatype. NOTE: This function needs to be
 *          kept in-sync with the parameters_struct data type in vic_def.h.
 *
 * @param mpi_type MPI_Datatype that can be used in MPI operations
 *****************************************************************************/
void
create_MPI_param_struct_type(MPI_Datatype *mpi_type)
{
    int           nitems; // number of elements in struct
    int           status;
    int          *blocklengths;
    size_t        i;
    MPI_Aint     *offsets;
    MPI_Datatype *mpi_types;

    // nitems has to equal the number of elements in parameters_struct
    nitems = 159;
    blocklengths = (int *) malloc(nitems * sizeof(int));
    if (blocklengths == NULL) {
        log_err("Memory allocation error in create_MPI_param_struct_type().")
    }

    offsets = (MPI_Aint *) malloc(nitems * sizeof(MPI_Aint));
    if (offsets == NULL) {
        log_err("Memory allocation error in create_MPI_param_struct_type().")
    }

    mpi_types = (MPI_Datatype *) malloc(nitems * sizeof(MPI_Datatype));
    if (mpi_types == NULL) {
        log_err("Memory allocation error in create_MPI_param_struct_type().")
    }

    // none of the elements in parameters_struct are arrays
    for (i = 0; i < (size_t) nitems; i++) {
        blocklengths[i] = 1;
    }

    // reset i
    i = 0;

    // double LAPSE_RATE;
    offsets[i] = offsetof(parameters_struct, LAPSE_RATE);
    mpi_types[i++] = MPI_DOUBLE;

    // double GAUGE_HEIGHT;
    offsets[i] = offsetof(parameters_struct, GAUGE_HEIGHT);
    mpi_types[i++] = MPI_DOUBLE;

    // double WIND_SPEED_DEFAULT;
    offsets[i] = offsetof(parameters_struct, WIND_SPEED_DEFAULT);
    mpi_types[i++] = MPI_DOUBLE;

    // double WIND_SPEED_MIN;
    offsets[i] = offsetof(parameters_struct, WIND_SPEED_MIN);
    mpi_types[i++] = MPI_DOUBLE;

    // double HUGE_RESIST;
    offsets[i] = offsetof(parameters_struct, HUGE_RESIST);
    mpi_types[i++] = MPI_DOUBLE;

    // double ALBEDO_BARE_SOIL;
    offsets[i] = offsetof(parameters_struct, ALBEDO_BARE_SOIL);
    mpi_types[i++] = MPI_DOUBLE;

    // double ALBEDO_H20_SURF;
    offsets[i] = offsetof(parameters_struct, ALBEDO_H20_SURF);
    mpi_types[i++] = MPI_DOUBLE;

    // double EMISS_GRND;
    offsets[i] = offsetof(parameters_struct, EMISS_GRND);
    mpi_types[i++] = MPI_DOUBLE;

    // double EMISS_VEG
    offsets[i] = offsetof(parameters_struct, EMISS_VEG);
    mpi_types[i++] = MPI_DOUBLE;

    // double EMISS_ICE
    offsets[i] = offsetof(parameters_struct, EMISS_ICE);
    mpi_types[i++] = MPI_DOUBLE;

    // double EMISS_SNOW
    offsets[i] = offsetof(parameters_struct, EMISS_SNOW);
    mpi_types[i++] = MPI_DOUBLE;

    // double EMISS_H2O
    offsets[i] = offsetof(parameters_struct, EMISS_H2O);
    mpi_types[i++] = MPI_DOUBLE;

    // double SOIL_RESID_MOIST
    offsets[i] = offsetof(parameters_struct, SOIL_RESID_MOIST);
    mpi_types[i++] = MPI_DOUBLE;

    // double SOIL_SLAB_MOIST_FRACT
    offsets[i] = offsetof(parameters_struct, SOIL_SLAB_MOIST_FRACT);
    mpi_types[i++] = MPI_DOUBLE;

    // double VEG_LAI_SNOW_MULTIPLIER
    offsets[i] = offsetof(parameters_struct, VEG_LAI_SNOW_MULTIPLIER);
    mpi_types[i++] = MPI_DOUBLE;

    // double VEG_MIN_INTERCEPTION_STORAGE
    offsets[i] = offsetof(parameters_struct, VEG_MIN_INTERCEPTION_STORAGE);
    mpi_types[i++] = MPI_DOUBLE;

    // double VEG_LAI_WATER_FACTOR
    offsets[i] = offsetof(parameters_struct, VEG_LAI_WATER_FACTOR);
    mpi_types[i++] = MPI_DOUBLE;

    // double CANOPY_CLOSURE
    offsets[i] = offsetof(parameters_struct, CANOPY_CLOSURE);
    mpi_types[i++] = MPI_DOUBLE;

    // double CANOPY_RSMAX
    offsets[i] = offsetof(parameters_struct, CANOPY_RSMAX);
    mpi_types[i++] = MPI_DOUBLE;

    // double CANOPY_VPDMINFACTOR
    offsets[i] = offsetof(parameters_struct, CANOPY_VPDMINFACTOR);
    mpi_types[i++] = MPI_DOUBLE;

    // double MTCLIM_TDAYCOEF
    offsets[i] = offsetof(parameters_struct, MTCLIM_TDAYCOEF);
    mpi_types[i++] = MPI_DOUBLE;

    // double MTCLIM_SOLAR_CONSTANT
    offsets[i] = offsetof(parameters_struct, MTCLIM_SOLAR_CONSTANT);
    mpi_types[i++] = MPI_DOUBLE;

    // double MTCLIM_SNOW_TCRIT
    offsets[i] = offsetof(parameters_struct, MTCLIM_SNOW_TCRIT);
    mpi_types[i++] = MPI_DOUBLE;

    // double MTCLIM_SNOW_TRATE
    offsets[i] = offsetof(parameters_struct, MTCLIM_SNOW_TRATE);
    mpi_types[i++] = MPI_DOUBLE;

    // double MTCLIM_TBASE
    offsets[i] = offsetof(parameters_struct, MTCLIM_TBASE);
    mpi_types[i++] = MPI_DOUBLE;

    // double MTCLIM_ABASE
    offsets[i] = offsetof(parameters_struct, MTCLIM_ABASE);
    mpi_types[i++] = MPI_DOUBLE;

    // double MTCLIM_C
    offsets[i] = offsetof(parameters_struct, MTCLIM_C);
    mpi_types[i++] = MPI_DOUBLE;

    // double MTCLIM_B0
    offsets[i] = offsetof(parameters_struct, MTCLIM_B0);
    mpi_types[i++] = MPI_DOUBLE;

    // double MTCLIM_B1
    offsets[i] = offsetof(parameters_struct, MTCLIM_B1);
    mpi_types[i++] = MPI_DOUBLE;

    // double MTCLIM_B2
    offsets[i] = offsetof(parameters_struct, MTCLIM_B2);
    mpi_types[i++] = MPI_DOUBLE;

    // double MTCLIM_RAIN_SCALAR
    offsets[i] = offsetof(parameters_struct, MTCLIM_RAIN_SCALAR);
    mpi_types[i++] = MPI_DOUBLE;

    // double MTCLIM_DIF_ALB
    offsets[i] = offsetof(parameters_struct, MTCLIM_DIF_ALB);
    mpi_types[i++] = MPI_DOUBLE;

    // double MTCLIM_SC_INT
    offsets[i] = offsetof(parameters_struct, MTCLIM_SC_INT);
    mpi_types[i++] = MPI_DOUBLE;

    // double MTCLIM_SC_SLOPE
    offsets[i] = offsetof(parameters_struct, MTCLIM_SC_SLOPE);
    mpi_types[i++] = MPI_DOUBLE;

    // double MTCLIM_SRADDT
    offsets[i] = offsetof(parameters_struct, MTCLIM_SRADDT);
    mpi_types[i++] = MPI_DOUBLE;

    // double MTCLIM_SW_PREC_THRESH
    offsets[i] = offsetof(parameters_struct, MTCLIM_SW_PREC_THRESH);
    mpi_types[i++] = MPI_DOUBLE;

    // double LAKE_TMELT
    offsets[i] = offsetof(parameters_struct, LAKE_TMELT);
    mpi_types[i++] = MPI_DOUBLE;

    // double LAKE_MAX_SURFACE
    offsets[i] = offsetof(parameters_struct, LAKE_MAX_SURFACE);
    mpi_types[i++] = MPI_DOUBLE;

    // double LAKE_BETA
    offsets[i] = offsetof(parameters_struct, LAKE_BETA);
    mpi_types[i++] = MPI_DOUBLE;

    // double LAKE_FRACMIN
    offsets[i] = offsetof(parameters_struct, LAKE_FRACMIN);
    mpi_types[i++] = MPI_DOUBLE;

    // double LAKE_FRACLIM
    offsets[i] = offsetof(parameters_struct, LAKE_FRACLIM);
    mpi_types[i++] = MPI_DOUBLE;

    // double LAKE_DM
    offsets[i] = offsetof(parameters_struct, LAKE_DM);
    mpi_types[i++] = MPI_DOUBLE;

    // double LAKE_SNOWCRIT
    offsets[i] = offsetof(parameters_struct, LAKE_SNOWCRIT);
    mpi_types[i++] = MPI_DOUBLE;

    // double LAKE_ZWATER
    offsets[i] = offsetof(parameters_struct, LAKE_ZWATER);
    mpi_types[i++] = MPI_DOUBLE;

    // double LAKE_ZSNOW
    offsets[i] = offsetof(parameters_struct, LAKE_ZSNOW);
    mpi_types[i++] = MPI_DOUBLE;

    // double LAKE_RHOSNOW
    offsets[i] = offsetof(parameters_struct, LAKE_RHOSNOW);
    mpi_types[i++] = MPI_DOUBLE;

    // double LAKE_CONDI
    offsets[i] = offsetof(parameters_struct, LAKE_CONDI);
    mpi_types[i++] = MPI_DOUBLE;

    // double LAKE_CONDS
    offsets[i] = offsetof(parameters_struct, LAKE_CONDS);
    mpi_types[i++] = MPI_DOUBLE;

    // double LAKE_LAMISW
    offsets[i] = offsetof(parameters_struct, LAKE_LAMISW);
    mpi_types[i++] = MPI_DOUBLE;

    // double LAKE_LAMILW
    offsets[i] = offsetof(parameters_struct, LAKE_LAMILW);
    mpi_types[i++] = MPI_DOUBLE;

    // double LAKE_LAMSSW
    offsets[i] = offsetof(parameters_struct, LAKE_LAMSSW);
    mpi_types[i++] = MPI_DOUBLE;

    // double LAKE_LAMSLW
    offsets[i] = offsetof(parameters_struct, LAKE_LAMSLW);
    mpi_types[i++] = MPI_DOUBLE;

    // double LAKE_LAMWSW
    offsets[i] = offsetof(parameters_struct, LAKE_LAMWSW);
    mpi_types[i++] = MPI_DOUBLE;

    // double LAKE_LAMWLW
    offsets[i] = offsetof(parameters_struct, LAKE_LAMWLW);
    mpi_types[i++] = MPI_DOUBLE;

    // double LAKE_A1
    offsets[i] = offsetof(parameters_struct, LAKE_A1);
    mpi_types[i++] = MPI_DOUBLE;

    // double LAKE_A2
    offsets[i] = offsetof(parameters_struct, LAKE_A2);
    mpi_types[i++] = MPI_DOUBLE;

    // double LAKE_QWTAU
    offsets[i] = offsetof(parameters_struct, LAKE_QWTAU);
    mpi_types[i++] = MPI_DOUBLE;

    // int LAKE_MAX_ITER
    offsets[i] = offsetof(parameters_struct, LAKE_MAX_ITER);
    mpi_types[i++] = MPI_INT;

    // double SVP_A
    offsets[i] = offsetof(parameters_struct, SVP_A);
    mpi_types[i++] = MPI_DOUBLE;

    // double SVP_B
    offsets[i] = offsetof(parameters_struct, SVP_B);
    mpi_types[i++] = MPI_DOUBLE;

    // double SVP_C
    offsets[i] = offsetof(parameters_struct, SVP_C);
    mpi_types[i++] = MPI_DOUBLE;

    // double CARBON_CATMCURRENT
    offsets[i] = offsetof(parameters_struct, CARBON_CATMCURRENT);
    mpi_types[i++] = MPI_DOUBLE;

    // double CARBON_SW2PAR
    offsets[i] = offsetof(parameters_struct, CARBON_SW2PAR);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_OMEGA
    offsets[i] = offsetof(parameters_struct, PHOTO_OMEGA);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_LAIMAX
    offsets[i] = offsetof(parameters_struct, PHOTO_LAIMAX);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_LAILIMIT
    offsets[i] = offsetof(parameters_struct, PHOTO_LAILIMIT);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_LAIMIN
    offsets[i] = offsetof(parameters_struct, PHOTO_LAIMIN);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_EPAR
    offsets[i] = offsetof(parameters_struct, PHOTO_EPAR);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_FCMAX
    offsets[i] = offsetof(parameters_struct, PHOTO_FCMAX);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_FCMIN
    offsets[i] = offsetof(parameters_struct, PHOTO_FCMIN);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_ZENITHMIN
    offsets[i] = offsetof(parameters_struct, PHOTO_ZENITHMIN);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_ZENITHMINPAR
    offsets[i] = offsetof(parameters_struct, PHOTO_ZENITHMINPAR);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_ALBSOIPARMIN
    offsets[i] = offsetof(parameters_struct, PHOTO_ALBSOIPARMIN);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_MINMAXETRANS
    offsets[i] = offsetof(parameters_struct, PHOTO_MINMAXETRANS);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_MINSTOMCOND
    offsets[i] = offsetof(parameters_struct, PHOTO_MINSTOMCOND);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_FCI1C3
    offsets[i] = offsetof(parameters_struct, PHOTO_FCI1C3);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_FCI1C4
    offsets[i] = offsetof(parameters_struct, PHOTO_FCI1C4);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_OX
    offsets[i] = offsetof(parameters_struct, PHOTO_OX);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_KCPHOTO_KC
    offsets[i] = offsetof(parameters_struct, PHOTO_KC);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_KO
    offsets[i] = offsetof(parameters_struct, PHOTO_KO);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_EC
    offsets[i] = offsetof(parameters_struct, PHOTO_EC);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_EO
    offsets[i] = offsetof(parameters_struct, PHOTO_EO);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_EV
    offsets[i] = offsetof(parameters_struct, PHOTO_EV);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_ER
    offsets[i] = offsetof(parameters_struct, PHOTO_ER);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_ALC3
    offsets[i] = offsetof(parameters_struct, PHOTO_ALC3);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_FRDC3
    offsets[i] = offsetof(parameters_struct, PHOTO_FRDC3);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_EK
    offsets[i] = offsetof(parameters_struct, PHOTO_EK);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_ALC4
    offsets[i] = offsetof(parameters_struct, PHOTO_ALC4);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_FRDC4
    offsets[i] = offsetof(parameters_struct, PHOTO_FRDC4);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_THETA
    offsets[i] = offsetof(parameters_struct, PHOTO_THETA);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_FRLEAF
    offsets[i] = offsetof(parameters_struct, PHOTO_FRLEAF);
    mpi_types[i++] = MPI_DOUBLE;

    // double PHOTO_FRGROWTH
    offsets[i] = offsetof(parameters_struct, PHOTO_FRGROWTH);
    mpi_types[i++] = MPI_DOUBLE;

    // double SRESP_E0_LT
    offsets[i] = offsetof(parameters_struct, SRESP_E0_LT);
    mpi_types[i++] = MPI_DOUBLE;

    // double SRESP_T0_LT
    offsets[i] = offsetof(parameters_struct, SRESP_T0_LT);
    mpi_types[i++] = MPI_DOUBLE;

    // double SRESP_WMINFM
    offsets[i] = offsetof(parameters_struct, SRESP_WMINFM);
    mpi_types[i++] = MPI_DOUBLE;

    // double SRESP_WMAXFM
    offsets[i] = offsetof(parameters_struct, SRESP_WMAXFM);
    mpi_types[i++] = MPI_DOUBLE;

    // double SRESP_WOPTFM
    offsets[i] = offsetof(parameters_struct, SRESP_WOPTFM);
    mpi_types[i++] = MPI_DOUBLE;

    // double SRESP_RHSAT
    offsets[i] = offsetof(parameters_struct, SRESP_RHSAT);
    mpi_types[i++] = MPI_DOUBLE;

    // double SRESP_RFACTOR
    offsets[i] = offsetof(parameters_struct, SRESP_RFACTOR);
    mpi_types[i++] = MPI_DOUBLE;

    // double SRESP_TAULITTER
    offsets[i] = offsetof(parameters_struct, SRESP_TAULITTER);
    mpi_types[i++] = MPI_DOUBLE;

    // double SRESP_TAUINTER
    offsets[i] = offsetof(parameters_struct, SRESP_TAUINTER);
    mpi_types[i++] = MPI_DOUBLE;

    // double SRESP_TAUSLOW
    offsets[i] = offsetof(parameters_struct, SRESP_TAUSLOW);
    mpi_types[i++] = MPI_DOUBLE;

    // double SRESP_FAIR
    offsets[i] = offsetof(parameters_struct, SRESP_FAIR);
    mpi_types[i++] = MPI_DOUBLE;

    // double SRESP_FINTER
    offsets[i] = offsetof(parameters_struct, SRESP_FINTER);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_MAX_SURFACE_SWE
    offsets[i] = offsetof(parameters_struct, SNOW_MAX_SURFACE_SWE);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_LIQUID_WATER_CAPACITY
    offsets[i] = offsetof(parameters_struct, SNOW_LIQUID_WATER_CAPACITY);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_NEW_SNOW_DENSITY
    offsets[i] = offsetof(parameters_struct, SNOW_NEW_SNOW_DENSITY);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_DENS_DMLIMIT
    offsets[i] = offsetof(parameters_struct, SNOW_DENS_DMLIMIT);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_DENS_MAX_CHANGE
    offsets[i] = offsetof(parameters_struct, SNOW_DENS_MAX_CHANGE);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_DENS_ETA0
    offsets[i] = offsetof(parameters_struct, SNOW_DENS_ETA0);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_DENS_C1
    offsets[i] = offsetof(parameters_struct, SNOW_DENS_C1);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_DENS_C2
    offsets[i] = offsetof(parameters_struct, SNOW_DENS_C2);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_DENS_C5
    offsets[i] = offsetof(parameters_struct, SNOW_DENS_C5);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_DENS_C6
    offsets[i] = offsetof(parameters_struct, SNOW_DENS_C6);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_DENS_F
    offsets[i] = offsetof(parameters_struct, SNOW_DENS_F);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_MIN_SWQ_EB_THRES
    offsets[i] = offsetof(parameters_struct, SNOW_MIN_SWQ_EB_THRES);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_A1
    offsets[i] = offsetof(parameters_struct, SNOW_A1);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_A2
    offsets[i] = offsetof(parameters_struct, SNOW_A2);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_L1
    offsets[i] = offsetof(parameters_struct, SNOW_L1);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_L2
    offsets[i] = offsetof(parameters_struct, SNOW_L2);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_NEW_SNOW_ALB
    offsets[i] = offsetof(parameters_struct, SNOW_NEW_SNOW_ALB);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_ALB_ACCUM_A
    offsets[i] = offsetof(parameters_struct, SNOW_ALB_ACCUM_A);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_ALB_ACCUM_B
    offsets[i] = offsetof(parameters_struct, SNOW_ALB_ACCUM_B);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_ALB_THAW_A
    offsets[i] = offsetof(parameters_struct, SNOW_ALB_THAW_A);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_ALB_THAW_B
    offsets[i] = offsetof(parameters_struct, SNOW_ALB_THAW_B);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_TRACESNOW
    offsets[i] = offsetof(parameters_struct, SNOW_TRACESNOW);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_CONDUCT
    offsets[i] = offsetof(parameters_struct, SNOW_CONDUCT);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_MAX_SNOW_TEMP
    offsets[i] = offsetof(parameters_struct, SNOW_MAX_SNOW_TEMP);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_MIN_RAIN_TEMP
    offsets[i] = offsetof(parameters_struct, SNOW_MIN_RAIN_TEMP);
    mpi_types[i++] = MPI_DOUBLE;

    // double BLOWING_KA
    offsets[i] = offsetof(parameters_struct, BLOWING_KA);
    mpi_types[i++] = MPI_DOUBLE;

    // double BLOWING_CSALT
    offsets[i] = offsetof(parameters_struct, BLOWING_CSALT);
    mpi_types[i++] = MPI_DOUBLE;

    // double BLOWING_UTHRESH
    offsets[i] = offsetof(parameters_struct, BLOWING_UTHRESH);
    mpi_types[i++] = MPI_DOUBLE;

    // double BLOWING_KIN_VIS
    offsets[i] = offsetof(parameters_struct, BLOWING_KIN_VIS);
    mpi_types[i++] = MPI_DOUBLE;

    // int BLOWING_MAX_ITER
    offsets[i] = offsetof(parameters_struct, BLOWING_MAX_ITER);
    mpi_types[i++] = MPI_INT;

    // int BLOWING_K
    offsets[i] = offsetof(parameters_struct, BLOWING_K);
    mpi_types[i++] = MPI_INT;

    // double BLOWING_SETTLING
    offsets[i] = offsetof(parameters_struct, BLOWING_SETTLING);
    mpi_types[i++] = MPI_DOUBLE;

    // int BLOWING_NUMINCS
    offsets[i] = offsetof(parameters_struct, BLOWING_NUMINCS);
    mpi_types[i++] = MPI_INT;

    // double TREELINE_TEMPERATURE
    offsets[i] = offsetof(parameters_struct, TREELINE_TEMPERATURE);
    mpi_types[i++] = MPI_DOUBLE;

    // double SNOW_DT
    offsets[i] = offsetof(parameters_struct, SNOW_DT);
    mpi_types[i++] = MPI_DOUBLE;

    // double SURF_DT
    offsets[i] = offsetof(parameters_struct, SURF_DT);
    mpi_types[i++] = MPI_DOUBLE;

    // double SOIL_DT
    offsets[i] = offsetof(parameters_struct, SOIL_DT);
    mpi_types[i++] = MPI_DOUBLE;

    // double CANOPY_DT
    offsets[i] = offsetof(parameters_struct, CANOPY_DT);
    mpi_types[i++] = MPI_DOUBLE;

    // double CANOPY_VP
    offsets[i] = offsetof(parameters_struct, CANOPY_VP);
    mpi_types[i++] = MPI_DOUBLE;

    // double TOL_GRND
    offsets[i] = offsetof(parameters_struct, TOL_GRND);
    mpi_types[i++] = MPI_DOUBLE;

    // double TOL_OVER
    offsets[i] = offsetof(parameters_struct, TOL_OVER);
    mpi_types[i++] = MPI_DOUBLE;

    // int FROZEN_MAXITER
    offsets[i] = offsetof(parameters_struct, FROZEN_MAXITER);
    mpi_types[i++] = MPI_INT;

    // int NEWT_RAPH_MAXTRIAL
    offsets[i] = offsetof(parameters_struct, NEWT_RAPH_MAXTRIAL);
    mpi_types[i++] = MPI_INT;

    // double NEWT_RAPH_TOLX
    offsets[i] = offsetof(parameters_struct, NEWT_RAPH_TOLX);
    mpi_types[i++] = MPI_DOUBLE;

    // double NEWT_RAPH_TOLF
    offsets[i] = offsetof(parameters_struct, NEWT_RAPH_TOLF);
    mpi_types[i++] = MPI_DOUBLE;

    // double NEWT_RAPH_R_MAX
    offsets[i] = offsetof(parameters_struct, NEWT_RAPH_R_MAX);
    mpi_types[i++] = MPI_DOUBLE;

    // double NEWT_RAPH_R_MIN
    offsets[i] = offsetof(parameters_struct, NEWT_RAPH_R_MIN);
    mpi_types[i++] = MPI_DOUBLE;

    // double NEWT_RAPH_RELAX1
    offsets[i] = offsetof(parameters_struct, NEWT_RAPH_RELAX1);
    mpi_types[i++] = MPI_DOUBLE;

    // double NEWT_RAPH_RELAX2
    offsets[i] = offsetof(parameters_struct, NEWT_RAPH_RELAX2);
    mpi_types[i++] = MPI_DOUBLE;

    // double NEWT_RAPH_RELAX3
    offsets[i] = offsetof(parameters_struct, NEWT_RAPH_RELAX3);
    mpi_types[i++] = MPI_DOUBLE;

    // double NEWT_RAPH_EPS2
    offsets[i] = offsetof(parameters_struct, NEWT_RAPH_EPS2);
    mpi_types[i++] = MPI_DOUBLE;

    // int ROOT_BRENT_MAXTRIES
    offsets[i] = offsetof(parameters_struct, ROOT_BRENT_MAXTRIES);
    mpi_types[i++] = MPI_INT;

    // int ROOT_BRENT_MAXITER
    offsets[i] = offsetof(parameters_struct, ROOT_BRENT_MAXITER);
    mpi_types[i++] = MPI_INT;

    // double ROOT_BRENT_TSTEP
    offsets[i] = offsetof(parameters_struct, ROOT_BRENT_TSTEP);
    mpi_types[i++] = MPI_DOUBLE;

    // double ROOT_BRENT_T
    offsets[i] = offsetof(parameters_struct, ROOT_BRENT_T);
    mpi_types[i++] = MPI_DOUBLE;

    // make sure that the we have the right number of elements
    if (i != (size_t) nitems) {
        log_err("Miscount in create_MPI_param_struct_type(): "
                "%zd not equal to %d\n", i, nitems);
    }

    status = MPI_Type_create_struct(nitems, blocklengths, offsets, mpi_types,
                                    mpi_type);
    if (status != MPI_SUCCESS) {
        log_err("MPI error in create_MPI_param_struct_type(): %d\n", status);
    }
    status = MPI_Type_commit(mpi_type);
    if (status != MPI_SUCCESS) {
        log_err("MPI error in create_MPI_param_struct_type(): %d\n", status);
    }

    // cleanup
    free(blocklengths);
    free(offsets);
    free(mpi_types);
}

#ifdef VIC_MPI_SUPPORT_TEST

#include <vic_driver_shared.h>
#include <assert.h>

filenames_struct filenames;
filep_struct     filep;
FILE            *LOG_DEST;

/******************************************************************************
 * @brief   Test routine for VIC MPI support functions
 * @details Note: need to define VIC_MPI_SUPPORT_TEST to compile.
 *          For example (from drivers/shared/src):
 *          mpicc-mpich-mp -Wall -Wextra -o vic_mpi_support vic_log.c
 *          open_file.c vic_mpi_support.c -I ../include/
 *          -I ../../../vic_run/include/ -DVIC_MPI_SUPPORT_TEST
 *****************************************************************************/
int
main(int    argc,
     char **argv)
{
    int                 rank, size;
    int                 status;
    MPI_Datatype        mpi_option_struct_type;
    MPI_Datatype        mpi_global_struct_type;
    MPI_Datatype        mpi_param_struct_type;
    global_param_struct global;
    option_struct       option;
    parameters_struct   param;

    // Initialize Log Destination
    strcpy(filenames.log_path, "MISSING");
    initialize_log();

    status = MPI_Init(&argc, &argv);
    if (status != MPI_SUCCESS) {
        log_err("MPI error in main(): %d\n", status);
    }
    status = MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (status != MPI_SUCCESS) {
        log_err("MPI error in main(): %d\n", status);
    }
    status = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (status != MPI_SUCCESS) {
        log_err("MPI error in main(): %d\n", status);
    }

    create_MPI_global_struct_type(&mpi_global_struct_type);
    create_MPI_option_struct_type(&mpi_option_struct_type);
    create_MPI_param_struct_type(&mpi_param_struct_type);

    if (rank == 0) {
        global.forceskip[0] = 4321;
        global.forceskip[1] = 8765;
        option.CARBON = false;
        option.CLOSE_ENERGY = true;
        option.COMPUTE_TREELINE = true;
        option.CONTINUEONERROR = true;
        option.Nfrost = 1234;
        option.Nlakenode = 5678;
        option.PRT_HEADER = false;
        option.PRT_SNOW_BAND = true;
        param.ROOT_BRENT_MAXTRIES = 6543;
        param.ROOT_BRENT_MAXITER = 1010;
        param.TOL_GRND = 0.001;
    }

    status = MPI_Bcast(&global, 1, mpi_global_struct_type,
                       0, MPI_COMM_WORLD);
    if (status != MPI_SUCCESS) {
        log_err("MPI error in main(): %d\n", status);
    }

    status = MPI_Bcast(&option, 1, mpi_option_struct_type,
                       0, MPI_COMM_WORLD);
    if (status != MPI_SUCCESS) {
        log_err("MPI error in main(): %d\n", status);
    }

    status = MPI_Bcast(&param, 1, mpi_param_struct_type,
                       0, MPI_COMM_WORLD);
    if (status != MPI_SUCCESS) {
        log_err("MPI error in main(): %d\n", status);
    }

    printf("%d: global.forceskip == %d\n", rank, global.forceskip[0]);
    assert(global.forceskip[0] == 4321);
    printf("%d: global.forceskip == %d\n", rank, global.forceskip[1]);
    assert(global.forceskip[1] == 8765);
    printf("%d: option.CARBON == %d\n", rank, option.CARBON);
    assert(option.CARBON == false);
    printf("%d: option.CLOSE_ENERGY == %d\n", rank, option.CLOSE_ENERGY);
    assert(option.CLOSE_ENERGY == true);
    printf("%d: option.COMPUTE_TREELINE == %d\n", rank,
           option.COMPUTE_TREELINE);
    assert(option.COMPUTE_TREELINE == true);
    printf("%d: option.CONTINUEONERROR == %d\n", rank, option.CONTINUEONERROR);
    assert(option.CONTINUEONERROR == true);
    printf("%d: option.Nfrost == %zd\n", rank, option.Nfrost);
    assert(option.Nfrost == 1234);
    printf("%d: option.Nlakenode == %zd\n", rank, option.Nlakenode);
    assert(option.Nlakenode == 5678);
    printf("%d: option.PRT_HEADER == %d\n", rank, option.PRT_HEADER);
    assert(option.PRT_HEADER == false);
    printf("%d: option.PRT_SNOW_BAND == %d\n", rank, option.PRT_SNOW_BAND);
    assert(option.PRT_SNOW_BAND == true);
    printf("%d: param.ROOT_BRENT_MAXTRIES == %d\n",
           rank, param.ROOT_BRENT_MAXTRIES);
    assert(param.ROOT_BRENT_MAXTRIES == 6543);
    printf("%d: param.ROOT_BRENT_MAXITER == %d\n",
           rank, param.ROOT_BRENT_MAXITER);
    assert(param.ROOT_BRENT_MAXITER == 1010);
    printf("%d: param.TOL_GRND == %lf\n", rank, param.TOL_GRND);
    assert(param.TOL_GRND == 0.001);


    status = MPI_Finalize();
    if (status != MPI_SUCCESS) {
        log_err("MPI error in main(): %d\n", status);
    }

    return EXIT_SUCCESS;
}

#endif