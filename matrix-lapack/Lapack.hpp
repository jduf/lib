#ifndef DEF_MLAPACK
#define DEF_MLAPACK

#include "Matrix.hpp"

//work for a symmetric matrix
/*{*/
extern "C" void dsyev_( /*eigensystem of a real symmetric matrix*/
		char const& jobz,
		char const& uplo,
		unsigned int const& n,
		double *m,
		unsigned int const& ldm,
		double *w,
		double *work,
		int const& lwork,
		int& info
		);
extern "C" void zheev_( /*eigensystem of a complex hermitian matrix*/
		char const& jobz,
		char const& uplo,
		unsigned int const& n,
		std::complex<double> *m,
		unsigned int const& ldm,
		double *w,
		std::complex<double> *work,
		int const& lwork,
		double *rwork,
		int& info
		);
extern "C" void dgeev_( /*eigensystem of a real general matrix*/
		char const& jobvl,
		char const& jobvr,
		unsigned int const& n,
		double *m,
		unsigned int const& ldm,
		double *wr,
		double *wi,
		double *vl,
		unsigned int const& ldvl,
		double *vr,
		unsigned int const& ldvr,
		double *work,
		int const& lwork,
		int& info
		);

extern "C" void dgetrf_(
		unsigned int const& row,
		unsigned int const& col,
		double *m,
		unsigned int const& ldm,
		int *ipiv,
		int& info
		);
extern "C" void zgetrf_(
		unsigned int const& row,
		unsigned int const& col,
		std::complex<double> *m,
		unsigned int const& ldm,
		int *ipiv,
		int& info
		);

extern "C" void dgetri_(
		unsigned int const& n,
		double *m,
		unsigned int const& ldm,
		int *ipiv,
		double *work,
		int const& lwork,
		int& info
		);
extern "C" void zgetri_(
		unsigned int const& n,
		std::complex<double> *m,
		unsigned int const& ldm,
		int *ipiv,
		std::complex<double> *work,
		int const& lwork,
		int& info
		);

extern "C" void dgecon_(
		char const& norm,
		unsigned int const& n,
		double const *m, 
		unsigned int const& ldm,
		double const& anorm, 
		double& rcond, 
		double *work,
		int const *iwork,
		int& info
		);
extern "C" void zgecon_(
		char const& norm,
		unsigned int const& n,
		std::complex<double> const *m, 
		unsigned int const& ldm,
		double const& anorm, 
		double& rcond, 
		std::complex<double> *work,
		double *rwork,
		int& info
		);

extern "C" double dlange_(
		char const& norm,
		unsigned int const& row,
		unsigned int const& col,
		double const *m,
		unsigned int const& ldm,
		double *work
		);
extern "C" double zlange_(
		char const& norm,
		unsigned int const& row,
		unsigned int const& col,
		std::complex<double> const *m,
		unsigned int const& ldm,
		double *work
		);

/*!start the A=QRP factorisation, gives directly R and P*/
extern "C" void dgeqp3_(
		unsigned int const& row,
		unsigned int const& col,
		double *m,
		unsigned int const& ldm,
		int *jpvt,
		double *tau,
		double *work,
		int const& lwork,
		int& info
		);
/*!ends the A=QRP factorisation, extract the Q matrix*/
extern "C" void dorgqr_(
		unsigned int const& row,
		unsigned int const& col,
		unsigned int const& k,
		double *m,
		unsigned int const& ldm,
		double *tau,
		double *work,
		int const& lwork,
		int& info
		);
/*}*/

/*!Class that allows an easy use of the LAPACK routines
 * 
 * - compute determinant
 * - compute inverse
 * - compute eigensystem
 * - compute LU decomposition
 * - compute QR decomposition
 * */
template<typename Type>
class Lapack{
	public:
		/*!Constructor that copy the input matrix, if use_new_matrix=false, the
		 * input matrix will be modified*/
		Lapack(Matrix<Type> *m, bool use_new_matrix, char matrix_type);
		/*!Destructor (delete m if use_new_matrix==true)*/
		~Lapack();

		/*!Specialized routine that computes the eigenvalues and the
		 * eigenvectors if EVec==true*/
		//template<typename M>
		//void eigensystem(Matrix<M>& EVal); 
		void eigensystem(Matrix<double>& EVal); 
		void eigensystem(Matrix<std::complex<double> >& EVal, Matrix<std::complex<double> >& EVec); 
		void eigenvalues(Matrix<double>& EVal); 
		void eigenvalues(Matrix<std::complex<double> >& EVal); 
		/*!Compute the determinant*/
		Type det();
		/*!Compute the LU decomposition*/
		void lu(Matrix<Type>& L, Matrix<Type>& U);
		/*!Compute the QR decomposition*/
		void qr(Matrix<Type>& Q, Matrix<Type>& R, bool permutation=false);
		/*!Compute the inverse*/
		void inv();
		void inv(Matrix<int>& ipiv);
		/*!Compute the norm of the matrix*/
		double norm();
		/*!Check if a matrix is singular*/
		Matrix<int> is_singular(double& rcn=0.0);

		Matrix<Type>* get_mat(){ return mat; }
		
	protected:
		Matrix<Type> *mat; //!< pointer on a Matrix
		bool use_new_matrix; //!< false if the original matrix will be overwritten by the LAPACK routines
		char const matrix_type; //!< type of matrix (symmetric, hermitian, general,...)
		
		/*!Specialized subroutine that calls a LAPACK routine to compute the
		 * LU decomposition*/
		void getrf(Matrix<int>& ipiv);
		/*!Specialized subroutine that calls a LAPACK routine to compute the
		 * inverse after the call of getrf*/
		void getri(Matrix<int>& ipiv);
		/*!Specialized subroutine that calls a LAPACK routine to compute the
		 * QR decomposition*/
		void geqp3(double* tau, int* jpvt);
		/*!Specialized subroutine that calls a LAPACK routine to compute the Q
		 * matrix of the QR decomposition after the call of geqp3*/
		void gqr(unsigned int k, double* tau);
		/*!Specialized subroutine that calls a LAPACK routine to compute the
		 * eigensystem of a symmetric real matrix*/
		void syev(Matrix<double>& EVal, char job);
		/*!Specialized subroutine that calls a LAPACK routine to compute the
		 * eigensystem of an hermitian complex matrix*/
		void heev(Matrix<double>& EVal, char job); 
		void geev(Matrix<std::complex<double> >& EVal, char job, Matrix<std::complex<double> >& EVec);
		/*!Specialized subroutine that calls a LAPACK routine to compute the
		 * norm of an general real matrix*/
		double lange(); 
		/*!Specialized subroutine that calls a LAPACK routine to compute the
		 * condition number of an general real matrix*/
		double gecon(double anorm); 

		/*!Forbids default constructor*/
		Lapack();
		/*!Forbids copy constructor*/
		Lapack(Lapack const& l);
		/*!Forbids assertion operator*/
		Lapack& operator=(Lapack const& l);
};

/*Constructors and destructor*/
/*{*/
template<typename Type>
Lapack<Type>::Lapack(Matrix<Type> *m, bool use_new_matrix, char matrix_type):
	mat(NULL),
	use_new_matrix(use_new_matrix),
	matrix_type(matrix_type)
{
	if(matrix_type == 'S' && m->row() != m->col()){ std::cerr<<"Lapack : constructor : the matix is not symmetric"<<std::endl; }
	if(use_new_matrix){ this->mat = new Matrix<Type>(*m); }
	else { this->mat = m; }
}

template<typename Type>
Lapack<Type>::~Lapack(){
	if(use_new_matrix){ delete this->mat; } 
}
/*}*/

/*methods used to call lapack*/
/*{*/
template<typename Type> 
Type Lapack<Type>::det()  {
	if (matrix_type != 'G'){
		std::cerr<<"Lapack : det : Matrix type "<<matrix_type<<" not implemented"<<std::endl;
		std::cerr<<"Lapack : det : the only matrix type implemented is G"<<std::endl;
		return 0;
	} else {
		/*! \warning the determinant needs to be rewritten for rectangles matrices  */
		Type d(1.0);
		unsigned int N(mat->row());
		Matrix<int> ipiv(N,1);
		getrf(ipiv);
		for(unsigned int i(0); i<N; i++){
			if(ipiv(i) != int(i+1)){ //! \note ipiv return value between [1,N]
				d *= - (*mat)(i,i);
			} else {
				d *= (*mat)(i,i);
			}
		}
		return d;
	}
}

template<typename Type>
void Lapack<Type>::inv(Matrix<int>& ipiv) {
	if (matrix_type != 'G'){
		std::cerr<<"Lapack : inv : Matrix type "<<matrix_type<<" not implemented"<<std::endl;
		std::cerr<<"Lapack : inv : the only matrix type implemented is G"<<std::endl;
	} else {
		if(ipiv.ptr()){ getri(ipiv); }
		else { std::cerr<<"Lapack : inv : the matrix is singular"<<std::endl;}
	}
}

template<typename Type>
void Lapack<Type>::inv() {
	if (matrix_type != 'G'){
		std::cerr<<"Lapack : inv : Matrix type "<<matrix_type<<" not implemented"<<std::endl;
		std::cerr<<"Lapack : inv : the only matrix type implemented is G"<<std::endl;
	} else {
		Matrix<int> ipiv(mat->row(),1);
		getrf(ipiv);
		getri(ipiv);
	}
}

template<typename Type>
void Lapack<Type>::lu(Matrix<Type>& L, Matrix<Type>& U)  {
	if (matrix_type != 'G'){
		std::cerr<<"Lapack : lu : Matrix type "<<matrix_type<<" not implemented"<<std::endl;
		std::cerr<<"Lapack : lu : the only matrix type implemented is G"<<std::endl;
	} else if (mat->row()!=mat->col()){
		std::cerr<<"Lapack : lu : need to be implemented for rectangle matrix"<<std::endl;
	} else  {
		std::cerr<<"Lapack : lu : could return P instead of having a void method"<<std::endl;
		unsigned int N(mat->row());
		L=Matrix<Type>(N,N);
		U=Matrix<Type>(N,N);
		Matrix<int> ipiv(N,1);
		getrf(ipiv);
		for(unsigned int i(0); i<N; i++){
			L(i,i)=1.0;
			U(i,i)=(*mat)(i,i);
			for(unsigned int j(i+1); j<N; j++){
				U(i,j) = (*mat)(i,j);
				L(j,i) = (*mat)(j,i);
			}
		}
	}
}

template<typename Type>
void Lapack<Type>::qr(Matrix<Type>& Q, Matrix<Type>& R, bool permutation)  {
	if (matrix_type != 'G'){
		std::cerr<<"Lapack : qr : Matrix type "<<matrix_type<<" not implemented"<<std::endl;
		std::cerr<<"Lapack : qr : the only matrix type implemented is G"<<std::endl;
	} else {
		unsigned int k(std::min(mat->row(),mat->col()));
		double *tau(new double[k]);
		int *jpvt(new int[mat->col()]); //! \warning contains element in [1,N]
		if(!permutation){
			for(unsigned int i(0); i<mat->col();i++){
				jpvt[i] = 1;
			}
		}

		geqp3(tau,jpvt);
		R.set(k,mat->col());
		for(unsigned int i(0);i<R.row();i++){
			for(unsigned int j(0);j<i;j++){
				R(i,j) = 0.0;
			}
			for(unsigned int j(i);j<R.col();j++){
				R(i,j) = (*mat)(i,j);
			}
		}
		gqr(k,tau);

		if(mat->col() > mat->row()){
			Q.set(mat->row(),mat->row());
			for(unsigned int i(0);i<Q.row();i++){
				for(unsigned int j(0);j<Q.col();j++){
					Q(i,j) = (*mat)(i,j);
				}
			}
		} else { 
			Q = *mat; 
		}

		if(permutation){
			mat->set(mat->col(),mat->col(),0.0);
			for(unsigned int i(0);i<mat->col();i++){
				(*mat)(i,jpvt[i]-1) = 1;
			}
		}

		delete[] tau;
	}
}

template<typename Type>
Matrix<int> Lapack<Type>::is_singular(double& rcn){
	Matrix<int> ipiv;
	if (matrix_type != 'G'){
		std::cerr<<"Lapack : is_singular : Matrix type "<<matrix_type<<" not implemented"<<std::endl;
		std::cerr<<"Lapack : is_singular : the only matrix type implemented is G"<<std::endl;
	} else if (mat->row()!=mat->col()){
		std::cerr<<"Lapack : is_singular : need to be checked for rectangle matrix"<<std::endl;
	} else {
		ipiv.set(std::min(mat->row(),mat->col()),1);
		double m_norm(lange());
		getrf(ipiv);

		rcn = gecon(m_norm);
		if(rcn<1e-10){
			std::cerr<<"Lapack : is_singular : reciproc_cond_numb="<<rcn<<std::endl;
			ipiv.set();//!set the output matrix to NULL pointer
		}
	}
	return ipiv;
}

template<typename Type> 
double Lapack<Type>::norm()  {
	if (matrix_type != 'G'){
		std::cerr<<"Lapack : norm : Matrix type "<<matrix_type<<" not implemented"<<std::endl;
		std::cerr<<"Lapack : norm : the only matrix type implemented is G"<<std::endl;
		return 0;
	} else {
		return lange();
	}
}

template<typename Type>
void Lapack<Type>::eigenvalues(Matrix<double>& EVal) {
	if(mat->row() != mat->col()){std::cerr<<"Lapack : eigensystem : matrix is not square"<<std::endl;}
	switch(matrix_type){
		case 'S':
			{
				syev(EVal,'N');
				break;
			}
		case 'H':
			{
				heev(EVal,'N');
				break;
			}
		default:
			{
				std::cerr<<"Lapack : eigensystem<double> : Matrix type "<<matrix_type<<" not implemented for real matrix"<<std::endl;
				std::cerr<<"Lapack : eigensystem<double> : the only matrix type implemented are G,S"<<std::endl;
				break;
			}
	}
}

template<typename Type>
void Lapack<Type>::eigenvalues(Matrix<std::complex<double> >& EVal) {
	if(mat->row() != mat->col()){std::cerr<<"Lapack : eigensystem : matrix is not square"<<std::endl;}
	switch(matrix_type){
		case 'G':
			{
				Matrix<std::complex<double> > tmp; /*not used if jobvr='N'*/
				geev(EVal,'N',tmp);
				break;
			}
		default:
			{
				std::cerr<<"Lapack : eigensystem<double> : Matrix type "<<matrix_type<<" not implemented for real matrix"<<std::endl;
				std::cerr<<"Lapack : eigensystem<double> : the only matrix type implemented are G,S"<<std::endl;
				break;
			}
	}
}

template<typename Type>
void Lapack<Type>::eigensystem(Matrix<double>& EVal) {
	if(mat->row() != mat->col()){std::cerr<<"Lapack : eigensystem : matrix is not square"<<std::endl;}
	switch(matrix_type){
		case 'S':
			{
				syev(EVal,'V');
				break;
			}
		case 'H':
			{
				heev(EVal,'V');
				break;
			}
		default:
			{
				std::cerr<<"Lapack : eigensystem<double> : Matrix type "<<matrix_type<<" not implemented for real matrix"<<std::endl;
				std::cerr<<"Lapack : eigensystem<double> : the only matrix type implemented are G,S"<<std::endl;
				break;
			}
	}
}

template<typename Type>
void Lapack<Type>::eigensystem(Matrix<std::complex<double> >& EVal, Matrix<std::complex<double> >& EVec) {
	if(mat->row() != mat->col()){std::cerr<<"Lapack : eigensystem : matrix is not square"<<std::endl;}
	switch(matrix_type){
		case 'G':
			{
				geev(EVal,'V',EVec);
				break;
			}
		default:
			{
				std::cerr<<"Lapack : eigensystem<double> : Matrix type "<<matrix_type<<" not implemented for real matrix"<<std::endl;
				std::cerr<<"Lapack : eigensystem<double> : the only matrix type implemented are G,S"<<std::endl;
				break;
			}
	}
}

/*}*/
#endif
