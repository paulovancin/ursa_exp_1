#include <matrix.h>

//----------------------------->  SET matrix  <---------------------------------

struct matrix set_values(int l, int c, fixed_t s[])
{
	int k = 0;
  struct matrix out;
  out.row = l;
  out.column = c;

	for(int i = 0; i < out.row; i++){
		for(int j = 0; j < out.column; j++){
			out.str[i][j] = s[k];
			k++;
		}
	}

  return out;
}


//--------------------------->  PRINT matrix  <---------------------------------

void print_matrix(struct matrix m)
{
    	for(int i = 0; i < m.row; i++){
				for(int j = 0; j < m.column; j++){
					fix_print(m.str[i][j]);
				}
				printf("\n");
			}
			printf("\n");
}

// //----------------------------->  GET row  <-----------------------------------

int get_row(struct matrix m)
{
	return m.row;
}

//---------------------------->  GET COLUMN  <----------------------------------

int get_column(struct matrix m)
{
	return m.column;
}

//------------------------->  GET A SINGLE VALUE  <-----------------------------

fixed_t get_value(struct matrix m, int row, int col)
{
    fixed_t VAL;
    VAL = m.str[row][col];
		return VAL;
}


//-------------------------> TRANSPOSED matrix <--------------------------------

struct matrix transposed(struct matrix m)
{

  struct matrix out;
  out.row = m.column;
  out.column = m.row;

	for (int i = 0; i < out.row; i++){
		for (int j = 0; j < out.column; j++){
			out.str[i][j] = m.str[j][i] ;
		}
	}

  return out;

}

//-------------------------> matrix SUM <---------------------------------------

struct matrix sum(struct matrix m1, struct matrix m2)
{

  struct matrix out;
  out.row = m1.row;
  out.column = m2.column;

	for (int i = 0; i < m1.row; i++){
		for (int j = 0; j < m1.column; j++){
			out.str[i][j] = m1.str[i][j] + m2.str[i][j];
		}
	}

  return out;

}


 //-------------------------> matrix SUBTRACTION <------------------------------

struct matrix subtraction(struct matrix m1, struct matrix m2)
{

  struct matrix out;
  out.row = m1.row;
  out.column = m1.column;

	for (int i = 0; i < out.row; i++){
		for (int j = 0; j < out.column; j++){
			out.str[i][j] = m1.str[i][j] - m2.str[i][j];
		}
	}

  return out;

}

//-----------------------> matrix MULTIPLICATION <------------------------------

struct matrix multiplication(struct matrix m1, struct matrix m2)
{

  struct matrix out;
  out.row = m1.row;
  out.column = m2.column;
  fixed_t auxm = fix_val(0.0);

	if (m1.column == m2.row){
		for (int i = 0; i < m1.row; i++){
			for (int j = 0; j < m2.column; j++){
				for (int x = 0; x < m2.row; x++){
					auxm = auxm + fix_mul(m1.str[i][x], m2.str[x][j]);
				}
				out.str[i][j] = auxm;
				auxm = 0;
			}
		}

    return out;
	}

		else
		{
			printf("Impossible to Multiplicate Those Matrices!!!\n");
		}

}

//----------------> MULTIPLICATION NUMBER = CTE * matrix  <---------------------

struct matrix multE(struct matrix m, fixed_t cte)
{
  struct matrix out;
  out.row = m.row;
  out.column = m.column;

	for (int i = 0; i < out.row; i++){
		for (int j = 0; j < out.column; j++){
			out.str[i][j] = fix_mul(m.str[i][j], cte);
		}
	}

  return out;

}


//--------------------> DIVISION = matrix / CTE  <------------------------------

struct matrix divE(struct matrix m, fixed_t cte)
{
  struct matrix out;
  out.row = m.row;
  out.column = m.column;

  if (cte == fix_val(0.00000)){
    cte = fix_val(0.000015);
  }

  if (cte == fix_val(-0.00000)){
    cte = fix_val(-0.000015);
  }

	for (int i = 0; i < out.row; i++){
		for (int j = 0; j < out.column; j++){
			out.str[i][j] = fix_div(m.str[i][j], cte);
		}
	}

  return out;

}



//------------------> SUM NUMBER = CTE * matrix  <-----------------------------

struct matrix sumE(struct matrix m, fixed_t cte)
{
  struct matrix out;
  out.row = m.row;
  out.column = m.column;

  for (int i = 0; i < m.row; i++){
    for (int j = 0; j < m.column; j++){
      out.str[i][j] = m.str[i][j] + cte;
    }
  }

  return out;

}


//-------------------------> matrix DETERMINANT  <------------------------------

fixed_t determinant(struct matrix m, int order)
{

  struct matrix CoFactor;

  fixed_t D = fix_val(0), D_aux;
  fixed_t sign = fix_val(1);

  if (order == 1){
      return (m.str[0][0]);
  }

  if (order > 1 && order <= 3){
      D = auxDet(m, order);
  }

  if (order >= 4){
    for (int f = 0; f < order; f++){
      CoFactor = cofactor(m, 0, f, order);
      D_aux = fix_mul(fix_mul(sign, m.str[0][f]), determinant(CoFactor, (order-1)));
      D = D + D_aux;
      sign = fix_mul(sign,fix_val(-1));
    }
  }

 return D;

}

//--------------------------> matrix CO-FACTOR  <-------------------------------


struct matrix cofactor(struct matrix m, int rw, int col, int order)
{

  int i = 0, j = 0;
  fixed_t temp[order][order];
  struct matrix out;

  for (int g = 0; g < order; g++){
    for(int h = 0; h < order; h++){
      if(g != rw && h != col){
        temp[i][j++] = m.str[g][h];
          if(j == order - 1){
            j = 0;
            i++;
          }
      }
    }
  }

  out.row = m.row;
  out.column = m.column;

  for(int h = 0; h < order; h++){
    for(int g = 0; g < order; g++){
      out.str[h][g] = temp[h][g];
    }
  }

  return out;

}


//------------------> AUXILIARY CALCULATION FOR DETERMINANT <-------------------

fixed_t auxDet(struct matrix m, int order)
{
  fixed_t D = 0, D_aux1, D_aux2;

  switch (order) {

    case 1:
    D = m.str[0][0];
    return D;
    break;

    case 2:
    D = fix_mul(m.str[0][0], m.str[1][1]) - fix_mul(m.str[0][1], m.str[1][0]);
    return D;
    break;

    case 3:
    D_aux1 = (fix_mul(fix_mul(m.str[0][0],m.str[1][1]), m.str[2][2])) - (fix_mul(fix_mul(m.str[0][0],m.str[1][2]), m.str[2][1])) + (fix_mul(fix_mul(m.str[0][1],m.str[1][2]), m.str[2][0]));
    D_aux2 = fix_mul(fix_val(-1),(fix_mul(fix_mul(m.str[0][1],m.str[1][0]), m.str[2][2]))) + (fix_mul(fix_mul(m.str[0][2],m.str[1][0]), m.str[2][1])) - (fix_mul(fix_mul(m.str[0][2],m.str[1][1]), m.str[2][0]));
    D = D_aux1 + D_aux2;
    return D;
    break;

  }

}


//---------------------------> matrix INVERSE  <--------------------------------

struct matrix inverse(struct matrix m)
{
  struct matrix b;
  struct matrix fac;
  struct matrix o_two;
  struct matrix aux;
  struct matrix out;

  int p,q,k,n,i,j;
  int order;
  fixed_t d, idet, det_aux, one;

  one = fix_val(1);
  order = m.row;

  if (order == 1){
    out.row = m.row;
    out.column = m.column;
    out.str[0][0] = fix_div(fix_val(1), m.str[0][0]);
  }


  if (order == 2){
    o_two.row = m.row;
    o_two.column = m.column;
    for (int i = 0; i < m.row; i++){
      for (int j = 0; j < m.column; j++){
        o_two.str[i][j] = m.str[i][j];
      }
    }
    det_aux = determinant(o_two, order);
    idet = fix_div(one, det_aux);
    aux = zeros(m.row, m.column);
    aux.str[0][0] = o_two.str[1][1];
    aux.str[0][1] = fix_mul(fix_val(-1),o_two.str[0][1]);
    aux.str[1][0] = fix_mul(fix_val(-1),o_two.str[1][0]);;
    aux.str[1][1] = o_two.str[0][0];
    out = multE(aux, idet);

  }

  if (order > 2){
    fixed_t Aux[order][order];
    out = zeros(order, order);
    b = zeros(order, order);
    fac = zeros(order, order);

    for (q = 0; q < order; q++){
      for (p = 0; p < order; p++){
        k = 0;
        n = 0;
        for (i = 0; i < order; i++){
          for (j = 0; j < order; j++){
            if (i != q && j != p){
              b.str[n][k] = m.str[j][i];
                if (n < (order-2)){
                  n++;
                }
                else{
                 n=0;
                 k++;
                }
            }
          }
        }

        if (p == 0 && q == 0){
          fac.str[p][q] = determinant(b, order-1);
        }
        else {

          if ((p + q) % 2 == 0){
            fac.str[p][q] = fix_mul(fix_val(1.0), determinant(b, order-1));
          }
          else {
            fac.str[p][q] = fix_mul(fix_val(-1.0), determinant(b, order-1));
          }
        }
       }
      }

      for (int g = 0; g < order; g++){
        for (int h = 0; h < order; h++){
          Aux[h][g] = fac.str[g][h];
        }
      }


      d = determinant(m, order);



      for (int g = 0; g < order; g++){
        for (int h = 0; h < order; h++){
          out.str[h][g] = fix_div(Aux[h][g], d);
        }
      }
    }

    return out;

}



//---------------------------> SET IDENTITY matrix <----------------------------

struct matrix setEye(int order)
{

  struct matrix out;
  out.row = order;
  out.column = order;

	for (int i = 0; i < order; i++){
		for (int j = 0; j < order; j++){
			if (j == i){
				out.str[i][j] = fix_val(1);
			}
			else{
				out.str[i][j] = fix_val(0);
			}
		}
	}

    return out;

}

//---------------------------> matrix OF ZEROS <--------------------------------

struct matrix zeros(int row, int col)
{
  struct matrix out;
  out.row = row;
  out.column = col;

	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			out.str[i][j] = fix_val(0);
		}
	}

  return out;
}


//---------------------------> matrix OF ONES <--------------------------------

struct matrix ones(int row, int col)
{
  struct matrix out;
  out.row = row;
  out.column = col;

	for(int i = 0; i < out.row; i++){
		for(int j = 0; j < out.column; j++){
			out.str[i][j] = fix_val(1);
		}
	}

  return out;
}


//------------------> matrix - CROSS PRODUCT 3X3 <------------------------------

struct matrix crossProduct3(fixed_t x, fixed_t y, fixed_t z)
{
  struct matrix out;
  out.row = 3;
  out.column = 3;
  out.str[0][0] = fix_val(0);
  out.str[0][1] = fix_mul(fix_val(-1),z);
  out.str[0][2] = y;
  out.str[1][0] = z;
  out.str[1][1] = fix_val(0);
  out.str[1][2] = fix_mul(fix_val(-1),x);
  out.str[2][0] = fix_mul(fix_val(-1),y);
  out.str[2][1] = x;
  out.str[2][2] = fix_val(0);

  return out;
}

//-------------------> BLOCK DIAGONAL CONCATENATION <---------------------------


//                               |A 0 0 .. 0|
// blkdiagN(A,B,C,...) produces  |0 B 0 .. 0|, CAUTION: Matrices must be square!
//                               |0 0 C .. 0|

//-------------> BLOCK DIAGONAL CONCATENATION: INPUT 2 MATRICES <---------------

struct matrix blkdiag2(struct matrix m1, struct matrix m2)
{
  struct matrix aux;
  struct matrix out;
  out.row = m1.row + m2.row;
  out.column = m1.column + m2.column;

  aux = zeros(out.row, out.column);

  for(int i = 0; i < m1.row; i++){
    for(int j = 0; j < m1.column; j++){
      aux.str[i][j] = m1.str[i][j];
    }
  }

  for(int i = 0; i < m2.row; i++){
    for(int j = 0; j < m2.column; j++){
      aux.str[i+m1.row][j+m1.column] = m2.str[i][j];
    }
  }

  for(int i = 0; i < out.row; i++){
    for(int j = 0; j < out.column; j++){
      out.str[i][j] = aux.str[i][j];
    }
  }

 return out;

}


//-------------> BLOCK DIAGONAL CONCATENATION: INPUT 3 MATRICES <---------------

struct matrix blkdiag3(struct matrix m1, struct matrix m2, struct matrix m3)
{

  struct matrix aux;
  struct matrix out;
  out.row = m1.row + m2.row + m3.row;
  out.column = m1.column + m2.column + m3.column;

  aux = zeros(out.row, out.column);

  for(int i = 0; i < m1.row; i++){
    for(int j = 0; j < m1.column; j++){
      aux.str[i][j] = m1.str[i][j];
    }
  }

  for(int i = 0; i < m2.row; i++){
    for(int j = 0; j < m2.column; j++){
      aux.str[i+m1.row][j+m1.column] = m2.str[i][j];
    }
  }

  for(int i = 0; i < m3.row; i++){
    for(int j = 0; j < m3.column; j++){
      aux.str[i+m1.row+m2.row][j+m1.column+m2.column] = m3.str[i][j];
    }
  }

  for(int i = 0; i < out.row; i++){
    for(int j = 0; j < out.column; j++){
      out.str[i][j] = aux.str[i][j];
    }
  }

  return out;

}

//-------------> BLOCK DIAGONAL CONCATENATION: INPUT 4 MATRICES <---------------

struct matrix blkdiag4(struct matrix m1, struct matrix m2, struct matrix m3, struct matrix m4)
{

  struct matrix aux;
  struct matrix out;
  out.row = m1.row + m2.row + m3.row + m4.row;
  out.column = m1.column + m2.column + m3.column + m4.column;

  aux = zeros(out.row, out.column);

  for(int i = 0; i < m1.row; i++){
    for(int j = 0; j < m1.column; j++){
      aux.str[i][j] = m1.str[i][j];
    }
  }

  for(int i = 0; i < m2.row; i++){
    for(int j = 0; j < m2.column; j++){
      aux.str[i+m1.row][j+m1.column] = m2.str[i][j];
    }
  }

  for(int i = 0; i < m3.row; i++){
    for(int j = 0; j < m3.column; j++){
      aux.str[i+m1.row+m2.row][j+m1.column+m2.column] = m3.str[i][j];
    }
  }

  for(int i = 0; i < m4.row; i++){
    for(int j = 0; j < m4.column; j++){
      aux.str[i+m1.row+m2.row+m3.row][j+m1.column+m2.column+m3.column] = m4.str[i][j];
    }
  }

  for(int i = 0; i < out.row; i++){
    for(int j = 0; j < out.column; j++){
      out.str[i][j] = aux.str[i][j];
    }
  }

  return out;

}



//-------------------> CUSTOM matrix OF MATRICES <------------------------------
//
//                    customMat(n_mats, vet_conf, A,B,C,D..)
//                where, vet_conf = [confX, confY, row, column]
//                produces:
//
//    |A B .. |
//    |C D .. |, confY and confX indicate how the matrix will be build. In case
//    |   ..  |  of confX = 2 and confY = 2, the matrices A, B, C and D will be
//               placed in a 2x2 grid. The parameters row and column will
//               indicate the total size of the matrix. Be aware of how the
//               matrices will be placed due to the differces of sizes, for now
//               will not be developed a restriction for placement.


struct matrix customMat(int n_mats, int conf[], ...)
{
  struct matrix aux;
  struct matrix out;

  int auxX = 0;
  int auxY = 0;

  out.row = conf[2];
  out.column = conf[3];


  va_list mats;
  va_start(mats, n_mats);


    for (int x = 0; x < conf[0]; x++){
      for (int y = 0; y < conf[1]; y++){
        aux = va_arg(mats, struct matrix);
          for (int i = 0; i < aux.row; i++){
            for (int j = 0; j < aux.column; j++){
              out.str[i + auxX][j + auxY] = aux.str[i][j];
            }
          }

          auxY = auxY + aux.column;
          if (auxY >= out.column){
          auxY = 0;
          }
        }

        auxX = auxX + aux.row;
        if (auxX >= out.row){
        auxX = 0;
        }
      }



  va_end(mats);
  return out;

}

//-------------------> COPY OF A matrix <------------------------------

struct matrix copy_matrix(struct matrix m)
{
  struct matrix out;
  out.row = m.row;
  out.column = m.column;

	for (int i = 0; i < out.row; i++){
		for (int j = 0; j < out.column; j++){
			out.str[i][j] = m.str[i][j] ;
		}
	}
  return out;
}

//-------------------------> RANK OF A matrix <---------------------------------

int rank(struct matrix m)
{
  int rank = m.column;
  fixed_t mult, temp;

  for (int row = 0; row < rank; row++){
    if (m.str[row][row] != fix_val(0)){
      for (int col = 0; col < m.row; col++){
        if (col != row){
          mult = fix_div(m.str[col][row], m.str[row][row]);
          for (int i = 0; i < rank; i++){
            m.str[col][i] -= fix_mul(mult, m.str[row][i]);
          }
        }
      }
    }
    else{
      int reduce = 1;
      for (int i = row + 1; i < m.row; i++){
        if (m.str[i][row] != fix_val(0)){
          for (int j = 0; j < rank; j++){
            temp = m.str[row][j];
            m.str[row][j] = m.str[i][j];
            m.str[i][j] = temp;
          }
          reduce = 0;
          break;
        }
      }
      if (reduce == 1){
        rank--;
        for (int i = 0; i < m.row; i++){
          m.str[i][row] = m.str[i][rank];
        }
      }
      row--;
    }
  }

  return rank;
}

//---------------------> CHECK SYMMETRY OF A matrix <---------------------------

int checkSymmetry(struct matrix m)
{
  int sym;
  int sum = 0, check;

  struct matrix cp;
  struct matrix tr;

  cp = zeros(m.row, m.column);
  tr = zeros(m.row, m.column);

  if (m.row != m.column){
    sym = 0;
  }

  else{
    for(int i = 0; i < m.row; i++){
      for(int j = 0; j < m.column; j++){
        cp.str[i][j] = m.str[i][j];
      }
    }

    check = m.row * m.column;
    tr = transposed(cp);



    for(int i = 0; i < m.row; i++){
      for(int j = 0; j < m.column; j++){
        if (cp.str[i][j] == tr.str[i][j]){
          sum++;
        }
      }
    }

    if (check == sum){
      sym = 1;
    }

    else{
      sym = 0;
    }
  }

  if (sym == 0){
    printf("---->Input matrix is NOT Symmetric!!!<----\n");
  }

  else{
    printf("---->Input matrix IS Symmetric!!!<----\n");
  }

  return sym;
}

//--------> DECOMPOSE A matrix INTO LOWER AND UPPER TRINGULAR MATRICES <--------

void luDecomposition(struct matrix m, struct matrix* L, struct matrix* U)
{
  fixed_t aux, aux2;

  L->row = m.row;
  L->column = m.column;

  U->row = m.row;
  U->column = m.column;

  for (int i = 0; i < m.row; i++){
    for (int j = 0; j < m.column; j++){
      L->str[i][j] = fix_val(0) ;
    }
  }

  for (int i = 0; i < m.row; i++){
    for (int j = 0; j < m.column; j++){
      U->str[i][j] = fix_val(0) ;
    }
  }

  int i = 0, j = 0, k = 0;

  for (i = 0; i < m.row; i++) {
    for (j = 0; j < m.row; j++) {
      if (j < i)
        L->str[j][i] = fix_val(0);
      else {
        L->str[j][i] = m.str[j][i];
        for (k = 0; k < i; k++) {
          L->str[j][i] = L->str[j][i] - fix_mul(L->str[j][k], U->str[k][i]);
        }
      }
    }
    for (j = 0; j < m.row; j++) {
      if (j < i){
        U->str[i][j] = fix_val(0);
      }
      else if (j == i){
        U->str[i][j] = fix_val(1);
      }
      else {
        U->str[i][j] = fix_div(m.str[i][j], L->str[i][i]);
        for (k = 0; k < i; k++) {
          aux = fix_mul(L->str[i][k], U->str[k][j]);
          aux2 = fix_div(aux, L->str[i][i]);
          U->str[i][j] = U->str[i][j] - aux2;
        }
      }
    }
  }
}


//-------------> QR DECOMPOSITION OF A SQUARE MATRIX <--------------------------


void qr(struct matrix m, struct matrix* Q, struct matrix* R)
{

struct matrix A, z, v, vt, P, P1, P2, P3, P4, I, Rc, Raux, Qc, Qaux, Qt, Rt, Qi, Ri;

fixed_t v1, normz, P2inv;

A.row = m.row;
A.column = m.column;

  for (int h = 0; h < m.row; h++){
    for (int g = 0; g < m.column; g++){
      A.str[h][g] = m.str[h][g] ;
    }
  }

  Qi = setEye(A.row);
  Ri = copy_matrix(A);


  for (int i = 0; i < A.column; i++){
    z = get_part(i, (A.row-1), i, i, Ri);
    fixed_t vectorz[z.row];

    for (int j = 0; j < z.row; j++){
      vectorz[j] = z.str[j][0];
    }


    normz = norm_vector(z.row,vectorz);
    v1 = fix_mul(fix_mul(fix_val(-1),sign(z.str[0][0])),normz) - z.str[0][0];


    v.row = z.row;
    v.column = z.column;

    for (int k = 0; k < z.row; k++){
      if (k == 0){
        v.str[k][0] = v1;
      }
      else{
        v.str[k][0] = fix_mul(fix_val(-1),z.str[k][0]);
      }
    }


    vt = transposed(v);
    P1 = multiplication(v,vt);
    P2 = multiplication(vt,v);
    P2inv = P2.str[0][0];
    P3 = divE(P1,P2inv);
    P4 = multE(P3, fix_val(2));
    I = setEye(A.row-i+1);
    P = subtraction(I,P4);


    Rc = get_part(i,A.row,0,A.column,Ri);
    Raux = multiplication(P,Rc);
    Qc = get_part(i,A.row,0,A.column,Qi);
    Qaux = multiplication(P,Qc);


    for(int g = i; g < A.row; g++){
      for(int h = 0; h < A.column; h++){
        Ri.str[g][h] = Raux.str[g-i][h];
      }
    }

  //
    for(int g = i; g < A.row; g++){
      for(int h = 0; h < A.column; h++){
        Qi.str[g][h] = Qaux.str[g-i][h];
      }
    }
    }


  Qt = transposed(Qi);
  Rt = triu(Ri);

  Q->row = Qt.row;
  Q->column = Qt.column;

  for (int i = 0; i < Qt.row; i++){
    for (int j = 0; j < Qt.column; j++){
      Q->str[i][j] = Qt.str[i][j] ;
    }
  }

  R->row = Rt.row;
  R->column = Rt.column;

  for (int i = 0; i < Rt.row; i++){
    for (int j = 0; j < Rt.column; j++){
      R->str[i][j] = Rt.str[i][j] ;
    }
  }



}


//------------------------> GET MATRIX DIAGONAL <-------------------------------

struct matrix diag(struct matrix m)
{

  struct matrix out;
  out = zeros(m.row, 1);

  for(int i = 0; i < m.row; i++){
    out.str[i][0] = m.str[i][i];
  }

  return out;

}


//------------------------> GET PART OF A MATRIX <------------------------------

struct matrix get_part(int ri, int rf, int ci, int cf, struct matrix m)
{
  struct matrix out;

  out.row = rf - ri + 1;
  out.column = cf - ci + 1;

  for (int i = 0; i < out.row; i++){
    for (int j = 0; j < out.column; j++){
      out.str[i][j] = m.str[i+ri][j+ci] ;
    }
  }

  return out;

}

//----------------------> HOUSEHOLDER DECOMPOSITION <---------------------------


void householder(struct matrix m, struct matrix* H, struct matrix* Q)
{

struct matrix x, A, A2, A3, ek, u, ut, uut, two_uut, u_aux1, u_aux2, I, Ii, P;
struct matrix Qt, Qtt, Qt2, Qi;


fixed_t sig1, nx, nu;

Qi = setEye(m.column);

A.row = m.row;
A.column = m.column;

for (int h = 0; h < m.row; h++){
  for (int g = 0; g < m.column; g++){
    A.str[h][g] = m.str[h][g] ;
  }
}


for(int i = 0; i < (m.column-2); i++){
  x = get_part(i+1,m.column-1,i,i,A);
  int s = get_row(x);
  ek = zeros(s,1);
  ek.str[0][0] = fix_val(1);


  fixed_t vector_x[s];
  int vectorx_size = 0;
  for (int a = 0; a < x.row; a++){
    for (int b = 0; b < x.column; b++){
      vector_x[vectorx_size] = x.str[a][b];
      vectorx_size += 1;
    }
  }

  sig1 = sign(x.str[0][0]);
  nx = norm_vector(s, vector_x);

  u_aux1 = multE(ek, fix_mul(sig1,nx));
  u_aux2 = sum(x,u_aux1);

  int t = get_row(x);
  fixed_t vector_u[t];
  int vectoru_size = 0;
  for (int a = 0; a < u_aux2.row; a++){
    for (int b = 0; b < u_aux2.column; b++){
      vector_u[vectoru_size] = u_aux2.str[a][b];
      vectoru_size += 1;
    }
  }

  nu = norm_vector(t, vector_u);
  u = divE(u_aux2, nu);


  I = setEye(s);
  ut = transposed(u);
  uut = multiplication(u,ut);
  two_uut = multE(uut, fix_val(2));
  P = subtraction(I, two_uut);


  Ii = setEye(i+1);
  Qt = blkdiag2(Ii,P);
  Qt2 = multiplication(Qt,Qi);
  Qtt = transposed(Qt);

  A2 = multiplication(Qtt,A);
  A3 = multiplication(A2,Qt);
  A = copy_matrix(A3);

  Q->row = Qt2.row;
  Q->column = Qt2.column;

  for (int i = 0; i < Qt2.row; i++){
    for (int j = 0; j < Qt2.column; j++){
      Qi.str[i][j] = Qt2.str[i][j] ;
    }
  }

  }

  H->row = A.row;
  H->column = A.column;

  for (int i = 0; i < A.row; i++){
    for (int j = 0; j < A.column; j++){
      H->str[i][j] = A.str[i][j] ;
    }
  }

  for (int i = 0; i < Qi.row; i++){
    for (int j = 0; j < Qi.column; j++){
      Q->str[i][j] = Qi.str[i][j] ;
    }
  }

}


//--------------------------> GET EIGENVALUES <---------------------------------

struct matrix eig(struct matrix m)
{

  int n = m.row;

  struct matrix H, q, r, eaux_in, eaux_out, out;

  H = copy_matrix(m);

  for (int i = 0; i < 100; i++){
    qr(H,&q,&r);
    H = multiplication(r,q);
  }


  int counter = 0;
  out.row = n;
  out.column = 1;



  for (int j = 0; j < n; j++){
    if (counter < n){
      if (H.str[counter+1][counter] <= fix_val(0.000015) && H.str[counter+1][counter] >= fix_val(-0.000015)){
        out.str[counter][0] = H.str[counter][counter];
        counter = counter + 1;
      }
      else{
        eaux_in.row = 2;
        eaux_in.column = 2;
        eaux_in.str[0][0] = H.str[counter][counter];
        eaux_in.str[0][1] = H.str[counter][counter+1];
        eaux_in.str[1][0] = H.str[counter+1][counter];
        eaux_in.str[1][1] = H.str[counter+1][counter+1];
        eaux_out = eig22(eaux_in);
        out.str[counter][0] = eaux_out.str[0][0];
        out.str[counter+1][0] = eaux_out.str[1][0];
        counter = counter + 2;

      }
    }
  }

  return out;

}

//-------------> ZEROS ALL THE ELEMENTS BELOW THE DIAGONAL <-------------------------

struct matrix triu(struct matrix m)
{
  struct matrix out;

  out.row = m.row;
  out.column = m.column;

  for (int i = 0; i < m.row; i++){
    for (int j = 0; j < m.column; j++){
      if (i > j){
        out.str[i][j] = fix_val(0);
      }
      else {
        out.str[i][j] = m.str[i][j];
      }
    }
  }

  return out;

}


//-------------> ZEROS ALL THE ELEMENTS ABOVE THE DIAGONAL <-------------------------

struct matrix tril(struct matrix m)
{
  struct matrix out;

  out.row = m.row;
  out.column = m.column;

  for (int i = 0; i < m.row; i++){
    for (int j = 0; j < m.column; j++){
      if (i < j){
        out.str[i][j] = fix_val(0);
      }
      else {
        out.str[i][j] = m.str[i][j];
      }
    }
  }

  return out;


}



//---------------------------> TRACE OF A MATRIX <--------------------------------

fixed_t trace(struct matrix m)
{

  fixed_t t;
  int n;

  n = m.row;
  t = fix_val(0);

  for (int i = 0; i < n; i++){
    t = t + m.str[i][i];
  }

  return t;


}


//---------------------------> EIGENVALUES OF A 2X2 MATRIX (AUXILIARY) <--------------------------------

struct matrix eig22(struct matrix m)
{

struct matrix out;

out.row = 2;
out.column = 1;

fixed_t t = trace(m);
fixed_t t2 = fix_pow(t,fix_val(2));
fixed_t det = determinant(m,2);
fixed_t y = t2-fix_mul(fix_val(4),det);
fixed_t x1 = fix_div((t+fix_sqrt(y)),fix_val(2));
fixed_t x2 = fix_div((t-fix_sqrt(y)),fix_val(2));


out.str[0][0] = x1;
out.str[1][0] = x2;

return out;

}




//----------------> INVERSE FUNCTION - ONLY 4X4 MATRICES <----------------------

struct matrix four_by_four_invert(struct matrix in)
{
	struct matrix out;
	fixed_t inv[16];
	fixed_t invOut[16];
	fixed_t det;
	fixed_t m[16];
	int count = 0;

	out.row = 4;
	out.column = 4;


		for(int x = 0; x < 4; x++){
			for(int y = 0; y < 4; y++){
				m[count] = in.str[x][y];
				count++;
			}
		}

   inv[0] = fix_mul(fix_mul(m[5],m[10]),m[15]) -
            fix_mul(fix_mul(m[5],m[11]),m[14]) -
            fix_mul(fix_mul(m[9],m[6]),m[15]) +
            fix_mul(fix_mul(m[9],m[7]),m[14]) +
            fix_mul(fix_mul(m[13],m[6]),m[11]) -
            fix_mul(fix_mul(m[13],m[7]),m[10]);


   inv[4] = fix_mul(fix_mul(fix_mul(m[4],fix_val(-1)), m[10]),m[15]) +
            fix_mul(fix_mul(m[4],m[11]),m[14]) +
            fix_mul(fix_mul(m[8],m[6]),m[15]) -
            fix_mul(fix_mul(m[8],m[7]),m[14]) -
            fix_mul(fix_mul(m[12],m[6]),m[11]) +
            fix_mul(fix_mul(m[12],m[7]),m[10]);

   inv[8] = fix_mul(fix_mul(m[4],m[9]),m[15]) -
            fix_mul(fix_mul(m[4],m[11]),m[13]) -
            fix_mul(fix_mul(m[8],m[5]),m[15]) +
            fix_mul(fix_mul(m[8],m[7]),m[13]) +
            fix_mul(fix_mul(m[12],m[5]),m[11]) -
            fix_mul(fix_mul(m[12],m[7]),m[9]);

   inv[12] = fix_mul(fix_mul(fix_mul(m[4],fix_val(-1)), m[9]),m[14]) +
             fix_mul(fix_mul(m[4],m[10]),m[13]) +
             fix_mul(fix_mul(m[8],m[5]),m[14]) -
             fix_mul(fix_mul(m[8],m[6]),m[13]) -
             fix_mul(fix_mul(m[12],m[5]),m[11]) +
             fix_mul(fix_mul(m[12],m[6]),m[9]);

   inv[1] = fix_mul(fix_mul(fix_mul(m[1],fix_val(-1)), m[10]),m[15]) +
            fix_mul(fix_mul(m[1],m[11]),m[14]) +
            fix_mul(fix_mul(m[9],m[2]),m[15]) -
            fix_mul(fix_mul(m[9],m[3]),m[14]) -
            fix_mul(fix_mul(m[13],m[2]),m[11]) +
            fix_mul(fix_mul(m[13],m[3]),m[10]);

   inv[5] = fix_mul(fix_mul(m[0],m[10]),m[15]) -
            fix_mul(fix_mul(m[0],m[11]),m[14]) -
            fix_mul(fix_mul(m[8],m[2]),m[15]) +
            fix_mul(fix_mul(m[8],m[3]),m[14]) +
            fix_mul(fix_mul(m[12],m[2]),m[11]) -
            fix_mul(fix_mul(m[12],m[3]),m[10]);

   inv[9] = fix_mul(fix_mul(fix_mul(m[0],fix_val(-1)), m[9]),m[15]) +
            fix_mul(fix_mul(m[0],m[11]),m[13]) +
            fix_mul(fix_mul(m[8],m[1]),m[15]) -
            fix_mul(fix_mul(m[8],m[3]),m[13]) -
            fix_mul(fix_mul(m[12],m[1]),m[11]) +
            fix_mul(fix_mul(m[12],m[3]),m[9]);

   inv[13] = fix_mul(fix_mul(m[0],m[9]),m[14]) -
             fix_mul(fix_mul(m[0],m[10]),m[13]) -
             fix_mul(fix_mul(m[8],m[1]),m[14]) +
             fix_mul(fix_mul(m[8],m[2]),m[13]) +
             fix_mul(fix_mul(m[12],m[1]),m[10]) -
             fix_mul(fix_mul(m[12],m[2]),m[9]);

   inv[2] = fix_mul(fix_mul(m[1],m[6]),m[15]) -
            fix_mul(fix_mul(m[1],m[7]),m[14]) -
            fix_mul(fix_mul(m[5],m[2]),m[15]) +
            fix_mul(fix_mul(m[5],m[3]),m[14]) +
            fix_mul(fix_mul(m[13],m[2]),m[7]) -
            fix_mul(fix_mul(m[13],m[3]),m[6]);

   inv[6] = fix_mul(fix_mul(fix_mul(m[0],fix_val(-1)), m[6]),m[15]) +
            fix_mul(fix_mul(m[0],m[7]),m[14]) +
            fix_mul(fix_mul(m[4],m[2]),m[15]) -
            fix_mul(fix_mul(m[4],m[3]),m[14]) -
            fix_mul(fix_mul(m[12],m[2]),m[7]) +
            fix_mul(fix_mul(m[12],m[3]),m[6]);

  inv[10] = fix_mul(fix_mul(m[0],m[5]),m[15]) -
            fix_mul(fix_mul(m[0],m[7]),m[13]) -
            fix_mul(fix_mul(m[4],m[1]),m[15]) +
            fix_mul(fix_mul(m[4],m[3]),m[13]) +
            fix_mul(fix_mul(m[12],m[1]),m[7]) -
            fix_mul(fix_mul(m[12],m[3]),m[5]);

  inv[14] = fix_mul(fix_mul(fix_mul(m[0],fix_val(-1)), m[5]),m[14]) +
            fix_mul(fix_mul(m[0],m[6]),m[13]) +
            fix_mul(fix_mul(m[4],m[1]),m[14]) -
            fix_mul(fix_mul(m[4],m[2]),m[13]) -
            fix_mul(fix_mul(m[12],m[1]),m[6]) +
            fix_mul(fix_mul(m[12],m[2]),m[5]);

  inv[3] = fix_mul(fix_mul(fix_mul(m[1],fix_val(-1)), m[6]),m[11]) +
           fix_mul(fix_mul(m[1],m[7]),m[10]) +
           fix_mul(fix_mul(m[5],m[2]),m[11]) -
           fix_mul(fix_mul(m[5],m[3]),m[10]) -
           fix_mul(fix_mul(m[9],m[2]),m[7]) +
           fix_mul(fix_mul(m[9],m[3]),m[6]);

  inv[7] = fix_mul(fix_mul(m[0],m[6]),m[11]) -
           fix_mul(fix_mul(m[0],m[7]),m[10]) -
           fix_mul(fix_mul(m[4],m[2]),m[11]) +
           fix_mul(fix_mul(m[4],m[3]),m[10]) +
           fix_mul(fix_mul(m[8],m[2]),m[7]) -
           fix_mul(fix_mul(m[8],m[3]),m[6]);

  inv[11] = fix_mul(fix_mul(fix_mul(m[0],fix_val(-1)), m[5]),m[11]) +
            fix_mul(fix_mul(m[0],m[7]),m[9]) +
            fix_mul(fix_mul(m[4],m[1]),m[11]) -
            fix_mul(fix_mul(m[4],m[3]),m[9]) -
            fix_mul(fix_mul(m[8],m[1]),m[7]) +
            fix_mul(fix_mul(m[8],m[3]),m[5]);

  inv[15] = fix_mul(fix_mul(m[0],m[5]),m[10]) -
            fix_mul(fix_mul(m[0],m[6]),m[9]) -
            fix_mul(fix_mul(m[4],m[1]),m[10]) +
            fix_mul(fix_mul(m[4],m[2]),m[9]) +
            fix_mul(fix_mul(m[8],m[1]),m[6]) -
            fix_mul(fix_mul(m[8],m[2]),m[5]);


    det = fix_mul(m[0], inv[0]) + fix_mul(m[1], inv[4]) + fix_mul(m[2], inv[8]) + fix_mul(m[3], inv[12]);
    det = fix_div(fix_val(1), det);



    for (int i = 0; i < 16; i++){
        invOut[i] = fix_mul(inv[i], det);
    }

    int k = 0;

    for(int i = 0; i < out.row; i++){
      for(int j = 0; j < out.column; j++){
        out.str[i][j] = invOut[k];
        k++;
      }
    }

      return out;
}


//-----------------------------> 2-NORM OF A MATRIX  <-------------------------------------------------

// typ_var twoNorm(struct matrix m){

//   typ_var sum, x, out;

//   for(int i = 0; i < row; i++){
//     for(int j = 0; j < column; j++){
//       x = Complex(fabs(str[i][j]),val(0));
//       sum += power(x,val(2));
//     }
//   }

//   out = square_root(sum);
//   return out;

// }
