
/* STATEMENT:

 GEOtop MODELS THE ENERGY AND WATER FLUXES AT THE LAND SURFACE
 GEOtop 1.225 'Moab' - 9 Mar 2012
 
 Copyright (c), 2012 - Stefano Endrizzi
 
 This file is part of GEOtop 1.225 'Moab'
 
 GEOtop 1.225 'Moab' is a free software and is distributed under GNU General Public License v. 3.0 <http://www.gnu.org/licenses/>
 WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE
 
 GEOtop 1.225 'Moab' is distributed as a free software in the hope to create and support a community of developers and users that constructively interact.
 If you just use the code, please give feedback to the authors and the community.
 Any way you use the model, may be the most trivial one, is significantly helpful for the future development of the GEOtop model. Any feedback will be highly appreciated.
 
 If you have satisfactorily used the code, please acknowledge the authors.
 
 */

#include "meteodata.h"
#include "config.h"

/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/

void time_interp_linear(double t0, double tbeg, double tend, double *out, double **data, long nlines, long ncols, long col_date, short flag, long *istart)

{

    short a0=0, abeg=0, aend=0;
    long i0, ibeg, iend, i, j;
    double t, add;

    i0   = find_line_data(flag, t0, *istart, data, col_date, nlines, &a0);
    ibeg = find_line_data(flag, tbeg, i0, data, col_date, nlines, &abeg);
    iend = find_line_data(flag, tend, ibeg, data, col_date, nlines, &aend);

    //printf("istart:%ld ibeg:%ld iend:%ld %f %f %ld %ld\n",*istart,ibeg,iend,tbeg,tend,abeg,aend);

    for (i=0; i<ncols; i++) {

        if ( (long)data[0][i] == number_absent) {

            out[i] = (double)number_absent;

        }else if (abeg == 1 && aend == 1) {

            out[i] = 0.;

            if ( (long)out[i] != number_novalue) {
                add = integrate_meas_linear_beh(flag, tbeg, ibeg+1, data, i, col_date);
                //printf("0:tbeg:%f %f\n",tbeg,add);
                if ((long)add != number_novalue) {
                    out[i] += add;
                }else {
                    out[i] = (double)number_novalue;
                }
            }

            if ( (long)out[i] != number_novalue) {
                add = -integrate_meas_linear_beh(flag, tend, iend+1, data, i, col_date);
                //printf("end:tend:%f %f\n",tend,add);
                if ((long)add != number_novalue) {
                    out[i] += add;
                }else {
                    out[i] = (double)number_novalue;
                }
            }

            j = ibeg+1;
            while ( (long)out[i] != number_novalue && j <= iend) {
                t = time_in_JDfrom0(flag, j, col_date, data);
                add = integrate_meas_linear_beh(flag, t, j+1, data, i, col_date);
                //printf("j:%ld:t:%f %f\n",j,t,add);
                j++;
                if ((long)add != number_novalue) {
                    out[i] += add;
                }else {
                    out[i] = (double)number_novalue;
                }

            }

            if ( (long)out[i] != number_novalue) {
                out[i] /= (tend	- tbeg);
            }

        }else {

            out[i] = (double)number_novalue;

        }
    }

    *istart = i0;
}

/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/

void time_interp_constant(double t0, double tbeg, double tend, double *out, double **data, long nlines, long ncols, long col_date, short flag, long *istart)

{

    short a0=0, abeg=0, aend=0;
    long i0, ibeg, iend, i, j;
    double t, add;

    i0   = find_line_data(flag, t0, *istart, data, col_date, nlines, &a0);
    ibeg = find_line_data(flag, tbeg, i0, data, col_date, nlines, &abeg);
    iend = find_line_data(flag, tend, ibeg, data, col_date, nlines, &aend);

    //printf("istart:%ld ibeg:%ld iend:%ld %f %f %d %d\n",*istart,ibeg,iend,tbeg,tend,abeg,aend);

    for (i=0; i<ncols; i++) {

        if ( (long)data[0][i] == number_absent) {

            out[i] = (double)number_absent;

        }else if (abeg == 1 && aend == 1) {

            out[i] = 0.;

            j = ibeg;

            while ( (long)out[i] != number_novalue && j < iend) {
                t = time_in_JDfrom0(flag, j+1, col_date, data);
                add = integrate_meas_constant_beh(flag, t, j+1, data, i, col_date);
                //printf("j:%ld t:%f int:%f va:%f\n",j,t,add,data[j+1][i]);
                j++;

                if ((long)add != number_novalue) {
                    out[i] += add;
                }else {
                    out[i] = (double)number_novalue;
                }
            }

            if ( (long)out[i] != number_novalue) {
                add = -integrate_meas_constant_beh(flag, tbeg, ibeg+1, data, i, col_date);
                //printf(":j:%ld t:%f int:%f va:%f\n",ibeg,tbeg,add);

                if ((long)add != number_novalue) {
                    out[i] += add;
                }else {
                    out[i] = (double)number_novalue;
                }
            }

            if ( (long)out[i] != number_novalue) {
                add = integrate_meas_constant_beh(flag, tend, iend+1, data, i, col_date);
                //printf("::j:%ld t:%f int:%f va:%f\n",iend,tend,add,data[iend+1][i]);

                if ((long)add != number_novalue) {
                    out[i] += add;
                }else {
                    out[i] = (double)number_novalue;
                }

            }

            if ( (long)out[i] != number_novalue) {
                out[i] /= (tend	- tbeg);
            }

            //printf("i:%ld out:%f\n\n",i,out[i]);

        }else {

            out[i] = (double)number_novalue;

            printf("<-i:%ld out:%f\n\n",i,out[i]);

        }

    }

    *istart = i0;
}

/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/

void time_no_interp(short flag, long *istart, double *out, double **data, long nlines, long ncols, long col_date, double tbeg)

{

    short abeg;
    long ibeg, i;

    ibeg = find_line_data(flag, tbeg+1.E-5, *istart, data, col_date, nlines, &abeg);

    if (abeg == 3){
        ibeg = nlines-1;
        abeg = 1;
    }

    for (i=0; i<ncols; i++) {

        if ( (long)data[0][i] == number_absent) {

            out[i] = (double)number_absent;

        }else if (abeg == 1 && (long)data[ibeg][i] != number_novalue) {

            out[i] = data[ibeg][i];

        }else {

            out[i] = (double)number_novalue;

        }
    }


    *istart = ibeg;
}


/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/

double integrate_meas_linear_beh(short flag, double t, long i, double **data, long col, long col_date){

    double t0, t1, value, res;
    FILE *f;

    if( (long)data[i  ][col] != number_novalue && (long)data[i  ][col] != number_absent &&
            (long)data[i-1][col] != number_novalue && (long)data[i-1][col] != number_absent ) {

        t0 = time_in_JDfrom0(flag, i-1, col_date, data);
        t1 = time_in_JDfrom0(flag, i  , col_date, data);

        if(fabs(t0-t1) < 1.E-5){
            f = fopen(FailedRunFile.c_str(), "w");
#ifdef USE_DOUBLE_PRECISION_OUTPUT
            fprintf(f, "Error:: There are 2 consecutive line in a meteo file with same date: t0:%12g(%ld) t1:%12g(%ld) equal",t0,i-1,t1,i);
#else
            fprintf(f, "Error:: There are 2 consecutive line in a meteo file with same date: t0:%f(%ld) t1:%f(%ld) equal",t0,i-1,t1,i);
#endif
            fclose(f);
            t_error("Fatal Error! Geotop is closed. See failing report.");
        }

        value = ( (t - t0) * data[i][col] + (t1 - t) * data[i-1][col] )/(t1 - t0);
        //printf("Integrate: t:%f t0:%f t1:%f v0:%f v1:%f v:%f\n",t,t0,t1,data[i-1][col],data[i][col],value);

        //area of trapezium
        res = 0.5 * ( value + data[i][col] ) * (t1 - t);

    }else {

        res = (double)number_novalue;

    }

    return(res);

}

/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/

double integrate_meas_constant_beh(short flag, double t, long i, double **data, long col, long col_date){

    double t0, value, res;

    if( (long)data[i][col] != number_novalue && (long)data[i][col] != number_absent ) {

        t0 = time_in_JDfrom0(flag, i-1, col_date, data);

        value = data[i][col];

        //area of rectangle
        res = value * (t - t0);

    }else {

        res = (double)number_novalue;

    }

    return(res);

}

/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/

long find_line_data(short flag, double t, long ibeg, double **data, long col_date, long nlines, short *a){

    long i;
    double t0, t1;

    i = ibeg;

    do{
        *a = 0;

        if (i+1 <= nlines-1) {

            t0 = time_in_JDfrom0(flag, i  , col_date, data);
            t1 = time_in_JDfrom0(flag, i+1, col_date, data);

            if (t0 <= t && t1 >= t){
                *a = 1;
            }else if (t0 > t){
                *a = 2;
            }

        }

        if (i+1 >= nlines-1 && *a == 0) *a = 3;

        if (*a == 0) i++;

    }while (*a == 0);

    return(i);

}

/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/

// All the times in the following subroutines are in Julian days from year 0

// flag 0: dates read in DDMMYYYYhhmm and converted in JDfrom0
// flag 1: dates in JDfrom0

double time_in_JDfrom0(short flag, long i, long col, double **data){

    double t;

    if (flag == 0) {
        t = convert_dateeur12_JDfrom0(data[i][col]);
    }else {
        t = data[i][col];
    }

    return(t);

}

/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/

long find_station(long metvar, long nstat, double **var){

    long i=0;

    while ( (long)var[i][metvar] == number_absent && i < nstat-1 ) {
        i++;
    }

    return i;
}

/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/

double **read_horizon(short a, long i, std::string name, char **ColDescr, long *num_lines, FILE *flog){

    FILE *f;
    long j;
    double **hor = NULL;
    std::string temp ;
    short fileyes;

    //check is the file exists
    if(name != string_novalue){
        fileyes=1;
    }else{
        fileyes=-1;
    }
    if(fileyes==1){
        temp = namefile_i(name,i);
        f=fopen(temp.c_str(),"r");
        if(f==NULL){
            fileyes=0;
        }else{
            fclose(f);
        }
    }

    //different cases
    if (fileyes == -1) {

        if(a==0){
            fprintf(flog,"\nWarning:: No horizon file found for point type #%ld. In this case the horizon will be considered always not obscured, i.e. shadow=FALSE\n",i);
            printf("\nWarning:: No horizon file found for point type #%ld. In this case the horizon will be considered always not obscured, i.e. shadow=FALSE\n",i);
        }else if (a==1){
            fprintf(flog,"\nWarning:: No horizon file found for meteo station #%ld. In this case the horizon will be considered always not obscured, i.e. shadow=FALSE\n",i);
            printf("\nWarning:: No horizon file found for meteo station #%ld. In this case the horizon will be considered always not obscured, i.e. shadow=FALSE\n",i);
        }

        *num_lines = 4;
        hor = (double**)malloc((*num_lines)*sizeof(double*));
        for( j=0; j<(*num_lines); j++){
            hor[j] = (double*)malloc(2*sizeof(double));
            hor[j][0] = 45.0+j*90.0;
            hor[j][1] = 0.0;
        }

    }else if (fileyes == 0) {

        if(a==0){
            fprintf(flog,"\nWarning:: No horizon file found for point type #%ld. In this case the horizon will be considered always not obscured, i.e. shadow=FALSE\n",i);
            printf("\nWarning:: No horizon file found for point type #%ld. In this case the horizon will be considered always not obscured, i.e. shadow=FALSE\n",i);
        }else if (a==1){
            fprintf(flog,"\nWarning:: No horizon file found for meteo station #%ld. In this case the horizon will be considered always not obscured, i.e. shadow=FALSE\n",i);
            printf("\nWarning:: No horizon file found for meteo station #%ld. In this case the horizon will be considered always not obscured, i.e. shadow=FALSE\n",i);
        }

        temp=namefile_i(name,i);

        f=fopen(temp.c_str(),"w");
        fprintf(f,"! Horizon file for met station or point #%ld \n",i);
        fprintf(f,"! All measures in degrees\n");
        fprintf(f,"\n");
        fprintf(f,"%s,%s\n",ColDescr[0],ColDescr[1]);

        *num_lines = 4;
        hor = (double**)malloc((*num_lines)*sizeof(double*));
        for( j=0; j<(*num_lines); j++){
            hor[j] = (double*)malloc(2*sizeof(double));
            hor[j][0] = 45.0+j*90.0;
            hor[j][1] = 0.0;
            fprintf(f,"%f,%f\n",hor[j][0],hor[j][1]);
        }

        fclose(f);

    } else if(fileyes == 1) {

        if(a==0){
            fprintf(flog,"\nHorizon file FOUND for point type #%ld\n",i);
            printf("\nHorizon file FOUND for point type #%ld\n",i);
        }else if (a==1) {
            fprintf(flog,"\nHorizon file FOUND for meteo station #%ld\n",i);
            printf("\nHorizon file FOUND for meteo station #%ld\n",i);
        }

        temp = namefile_i(name,i);
        hor = read_txt_matrix(temp, 33, 44, ColDescr, 2, num_lines, flog);

        if ( (long)hor[0][0] == number_absent || (long)hor[0][1] == number_absent) {
            f = fopen(FailedRunFile.c_str(), "w");
            fprintf(f, "Error:: In the file %s the columns %s and/or %s are missing\n",temp.c_str(),ColDescr[0],ColDescr[1]);
            fclose(f);
            t_error("Fatal Error! Geotop is closed. See failing report.");
        }

    }

    return(hor);

}

/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/

short fixing_dates(long imeteo, double **data, double ST, double STstat, long nlines, long date12col, long JDfrom0col){

    long i;
    FILE *f;

    if ( (long)data[0][JDfrom0col] == number_absent && (long)data[0][date12col] != number_absent) {

        for (i=0; i<nlines; i++) {
            //converting in JDfrom0
            data[i][JDfrom0col] = convert_dateeur12_JDfrom0(data[i][date12col]);
            //setting ST
            data[i][JDfrom0col] += (ST - STstat) / 24.;
        }

        return 1;

    }else if ( (long)data[0][JDfrom0col] != number_absent) {

        return 0;

    }else {

        f = fopen(FailedRunFile.c_str(), "w");
        fprintf(f, "Date and Time not available for meteo station %ld\n",imeteo);
        fclose(f);
        t_error("Fatal Error! Geotop is closed. See failing report.");

        return -1;

    }

}

/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/

short fill_wind_xy(double **data, long nlines, long Wspeed, long Wdir, long Wx, long Wy, char *HeaderWx, char *HeaderWy){

    long i;

    //if the columns Wspeed and Wdir are present, and the columns Wx and Wy are not present
    if ( (long)data[0][Wspeed] != number_absent && (long)data[0][Wdir] != number_absent && ( (long)data[0][Wx] == number_absent || (long)data[0][Wy] == number_absent ) ) {
        for (i=0; i<nlines; i++) {
            if ( (long)data[i][Wspeed] != number_novalue && (long)data[i][Wdir] != number_novalue ) {
                data[i][Wx] = -data[i][Wspeed] * sin(data[i][Wdir] * GTConst::Pi / 180.);
                data[i][Wy] = -data[i][Wspeed] * cos(data[i][Wdir] * GTConst::Pi / 180.);
            }else {
                data[i][Wx] = (double)number_novalue;
                data[i][Wy] = (double)number_novalue;
            }

            if(strcmp(HeaderWx,string_novalue)!=0 && strcmp(HeaderWy,string_novalue)!=0){
                data[i][Wspeed] = (double)number_absent;
                data[i][Wdir] = (double)number_absent;
            }
        }
        if(strcmp(HeaderWx,string_novalue)!=0 && strcmp(HeaderWy,string_novalue)!=0){
            return 1;
        }else {
            return 0;
        }
    }else {
        return 0;
    }

}

/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/

short fill_wind_dir(double **data, long nlines, long Wspeed, long Wdir, long Wx, long Wy, char *HeaderWSpeed, char *HeaderWdir){

    long i;
    double a;

    //if the columns Wspeed and Wdir are present, and the columns Wx and Wy are not present
    if ( (long)data[0][Wx] != number_absent && (long)data[0][Wy] != number_absent && ( (long)data[0][Wspeed] == number_absent || (long)data[0][Wdir] == number_absent ) ) {
        for (i=0; i<nlines; i++) {
            if ( (long)data[i][Wx] != number_novalue && (long)data[i][Wy] != number_novalue ) {

                data[i][Wspeed] = sqrt(pow(data[i][Wx], 2.)+pow(data[i][Wy], 2.));

                if (fabs(data[i][Wy]) < 1.E-10 ) {
                    a = GTConst::Pi/2.;
                }else {
                    a = atan(fabs(data[i][Wx]/data[i][Wy]));
                }

                if (data[i][Wx] <= 0 && data[i][Wy] <= 0 ) {
                    data[i][Wdir] = a*180./GTConst::Pi;
                }else if (data[i][Wx] <= 0 && data[i][Wy] >= 0 ) {
                    data[i][Wdir] = a*180./GTConst::Pi + 90.;
                }else if (data[i][Wx] >= 0 && data[i][Wy] >= 0 ) {
                    data[i][Wdir] = a*180./GTConst::Pi + 180.;
                }else {
                    data[i][Wdir] = a*180./GTConst::Pi + 270.;
                }

            }else {

                data[i][Wspeed] = (double)number_novalue;
                data[i][Wdir] = (double)number_novalue;

            }

            if(strcmp(HeaderWSpeed,string_novalue)!=0 && strcmp(HeaderWdir,string_novalue)!=0){
                data[i][Wx] = (double)number_absent;
                data[i][Wy] = (double)number_absent;
            }
        }
        if(strcmp(HeaderWSpeed,string_novalue)!=0 && strcmp(HeaderWdir,string_novalue)!=0){
            return 1;
        }else {
            return 0;
        }
    }else {
        return 0;
    }

}

/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/

short fill_Tdew(long imeteo, GeoVector<double> &Z, double **data, long nlines, long RH, long Tair, long Tairdew, char *HeaderTdew, double RHmin){

    long i;

    if ( (long)data[0][RH] != number_absent && (long)data[0][Tair] != number_absent && (long)data[0][Tairdew] == number_absent ) {
        for (i=0; i<nlines; i++) {
            if ( (long)data[i][RH] != number_novalue && (long)data[i][Tair] != number_novalue ) {
                data[i][Tairdew] = Tdew(data[i][Tair], Fmax(RHmin, data[i][RH])/100., Z[imeteo]);
            }else {
                data[i][Tairdew] = (double)number_novalue;
            }

            if (strcmp(HeaderTdew, string_novalue) != 0) {
                data[i][RH] = (double)number_absent;
            }
        }
        if (strcmp(HeaderTdew, string_novalue) != 0) {
            return 1;
        }else {
            return 0;
        }
    }else {
        return 0;
    }
}

/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/

short fill_RH(long imeteo, GeoVector<double> &Z, double **data, long nlines, long RH, long Tair, long Tairdew, char *HeaderRH){

    long i;

    if ( (long)data[0][RH] == number_absent && (long)data[0][Tair] != number_absent && (long)data[0][Tairdew] != number_absent ) {
        for (i=0; i<nlines; i++) {
            if ( (long)data[i][Tairdew] != number_novalue && (long)data[i][Tair] != number_novalue ) {
                data[i][RH] = 100.*RHfromTdew(data[i][Tair], data[i][Tairdew], Z[imeteo]);
            }else {
                data[i][RH] = (double)number_novalue;
            }

            if (strcmp(HeaderRH, string_novalue) != 0) {
                data[i][Tairdew] = (double)number_absent;
            }
        }
        if (strcmp(HeaderRH, string_novalue) != 0) {
            return 1;
        }else {
            return 0;
        }
    }else {
        return 0;
    }
}

/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/

short fill_Pint(long imeteo, double **data, long nlines, long Prec, long PrecInt, long JDfrom0, char *HeaderPrecInt){

    long i;

    if ( (long)data[0][Prec] != number_absent && (long)data[0][PrecInt] == number_absent ) {

        data[0][PrecInt] = (double)number_novalue;

        for (i=1; i<nlines; i++) {
            if ( (long)data[i][Prec] != number_novalue) {
                data[i][PrecInt] = data[i][Prec] / (data[i][JDfrom0] - data[i-1][JDfrom0]);//[mm/d]
                data[i][PrecInt] /= 24.;//[mm/h]
                //printf("%ld %f %f\n",i,data[i][PrecInt],data[i][Prec]);
            }else{
                data[i][PrecInt] = (double)number_novalue;
            }

            if (strcmp(HeaderPrecInt, string_novalue) != 0) {
                data[i][Prec] = (double)number_absent;
            }

        }

        if (strcmp(HeaderPrecInt, string_novalue) != 0) {
            return 1;
        }else {
            return 0;
        }

    }else {

        return 0;

    }
}

/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/

void check_times(long imeteo, double **data, long nlines, long JDfrom0){

    long i;
    FILE *f;

    for (i=1; i<nlines; i++) {
        if (data[i][JDfrom0] <= data[i-1][JDfrom0]) {
            f = fopen(FailedRunFile.c_str(), "w");
#ifdef USE_DOUBLE_PRECISION_OUTPUT
            fprintf(f, "Error:: Time %12g is before Time %12g of previous line in meteo file %ld at line %ld.\n",data[i][JDfrom0],data[i-1][JDfrom0],imeteo,i);
#else
            fprintf(f, "Error:: Time %f is before Time %f of previous line in meteo file %ld at line %ld.\n",data[i][JDfrom0],data[i-1][JDfrom0],imeteo,i);
#endif
            fclose(f);
            t_error("Fatal Error! Geotop is closed. See failing report.");
        }
    }
}

/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/


void rewrite_meteo_files(double **meteo, long meteolines, char **header, std::string name, short added_JD, short added_wind_xy, short added_wind_dir,
                         short added_cloudiness, short added_Tdew, short added_RH, short added_Pint){

    std::string newname;
    short first_column, write;
    long i, n, d, m, y, h, mi;
    FILE *f;

    newname = name ;
    newname += ".old" ;
    f=fopen(newname.c_str(),"r");
    if (f == NULL){
        write = 1;
    }else {
        fclose(f);
        write = 0;
    }

    if (added_cloudiness != 1 && added_wind_xy != 1 && added_wind_dir != 1 && added_JD != 1 && added_Tdew != 1 && added_RH != 1 && added_Pint != 1) write = 0;

    if (write == 1) {
        rename(name.c_str(), newname.c_str());

        f = fopen(name.c_str(), "w");

        first_column = 1;
        for (i=0; i<nmet; i++) {
            if ( (long)meteo[0][i] != number_absent && strcmp(header[i], string_novalue) != 0){
                if (first_column == 0) {
                    fprintf(f,",");
                }else {
                    first_column = 0;
                }
                fprintf(f, "%s", header[i]);
            }else if (i == iDate12) {
                if (first_column == 0) {
                    fprintf(f,",");
                }else {
                    first_column = 0;
                }
                fprintf(f, "%s", header[i]);
            }
        }
        fprintf(f, "\n");

        for (n=0; n<meteolines; n++) {
            first_column = 1;
            for (i=0; i<nmet; i++) {
                if ( (long)meteo[0][i] != number_absent && strcmp(header[i], string_novalue) != 0){
                    if (first_column == 0) {
                        fprintf(f,",");
                    }else {
                        first_column = 0;
                    }
                    if(i == iDate12) {
                        convert_dateeur12_daymonthyearhourmin(meteo[n][i], &d, &m, &y, &h, &mi);
                        fprintf(f, "%02.0f/%02.0f/%04.0f %02.0f:%02.0f",(float)d,(float)m,(float)y,(float)h,(float)mi);
                    }else{
#ifdef USE_DOUBLE_PRECISION_OUTPUT
                        fprintf(f, "%12g", meteo[n][i]);
#else
                        fprintf(f, "%f", meteo[n][i]);
#endif
                    }
                }else if (i == iDate12) {
                    if (first_column == 0) {
                        fprintf(f,",");
                    }else {
                        first_column = 0;
                    }
                    convert_dateeur12_daymonthyearhourmin(convert_JDfrom0_dateeur12(meteo[n][iJDfrom0]), &d, &m, &y, &h, &mi);
                    fprintf(f, "%02.0f/%02.0f/%04.0f %02.0f:%02.0f",(float)d,(float)m,(float)y,(float)h,(float)mi);
                }
            }
            fprintf(f, "\n");
        }

        fclose(f);
    }
}

/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
/******************************************************************************************************************************************/
