#include <stdio.h>

/***********************/
/* Project CSC 6114    */
/* Sathish Ramamoorthy */
/***********************/

#define M 3                           /* order of the input matrices */
#define N 5                           /* M - rows, N - columns       */

int A[M][N] = {                       /* Input Matrix A              */
   {10, 3, 2, 9, 1},
   {0, 0, 0, 2, 5},
   {3, 2, 993, 8, 135},
}; 
int B[M][N] = {                       /* Input Matrix B              */
   {0, 2, 233, 459, 0},
   {234, 0, 103, 34, 40},
   {9, 23, 3, 0, 3},
}; 

/*-----------------------------------------------------------------------------*/
/* Structure to be used as nodes in the linked list that go right on the       */
/* adjacency list                                                              */
/*-----------------------------------------------------------------------------*/
typedef struct n_Node{
   int n;
   int element;              /* array element */
   struct n_Node *next_n;
} n_node ;
/*-----------------------------------------------------------------------------*/
/* Structure to be used as nodes in the linked list that goes top to down      */
/* on the left hand side of the adjacency list.                                */
/*-----------------------------------------------------------------------------*/
typedef struct m_Node{
   struct m_Node *next_m;
   n_node *next_n;
} m_node;
/*-----------------------------------------------------------------------------*/
/* Structure to represent the array, it has its order and the starting m-node  */
/* arr is the starting point for the adjacency list representation of the array*/
/*-----------------------------------------------------------------------------*/
typedef struct the_Array{
   int no_of_rows;
   int no_of_cols;
   m_node *m;
} arr;

arr  *classical_to_adjacency(int[][]);
arr  *add_two_matrices(arr *, arr *);
void  adjacency_to_classical(arr *, int[][]);
void  print_classical_array(int[][]);
/*-----------------------------------------------------------------------------*/
/* main reads two arrays from the user, converts them to adjacency lists,      */
/* prints it out back to the screen in classical representation, computes      */
/* the sum of the two matrices using the adjacency lists and prints out the    */
/* result to the screen in classical form.                                     */
/*-----------------------------------------------------------------------------*/
main() {
   arr *array_A, *array_B, *sum_of_AB;
   int classical_A[M][N], classical_B[M][N],classical_sum_of_AB[M][N];

   printf("\n***************** Array A ********************\n");
   array_A = classical_to_adjacency(A);
   adjacency_to_classical(array_A, classical_A);
   print_classical_array(classical_A);

   printf("\n***************** Array B ********************\n");
   array_B = classical_to_adjacency(B);
   adjacency_to_classical(array_B, classical_B);
   print_classical_array(classical_B);

   printf("\n******* Result array of sum of A & B *********\n");
   sum_of_AB = add_two_matrices(array_A, array_B);
   adjacency_to_classical(sum_of_AB, classical_sum_of_AB);
   print_classical_array(classical_sum_of_AB);

   printf("\nOrder of the arrays : %i X %i\n\n", M, N);

   exit(0);
}
/*-----------------------------------------------------------------------------*/
/* Function to add the two matrices.                                           */
/*-----------------------------------------------------------------------------*/
arr *add_two_matrices(arr *arrA, arr *arrB){
   arr *result_arr;
   int sum = 0, n_of_A, n_of_B;
   m_node *current_mA = arrA->m, *current_mB = arrB->m, *current_m=NULL, *result_m;
   n_node *current_nA = NULL, *current_nB = NULL, *result_n, *current_n;
   result_arr = (arr *) malloc(sizeof *result_arr);
   while (current_mA != NULL && current_mB != NULL){
      result_m = (m_node *) malloc(sizeof *result_m);
      if (current_m == NULL) {
         result_arr->m = result_m;
      } else {
         current_m->next_m = result_m;
      }
      current_m = result_m;
      current_m->next_m = NULL;
      current_m->next_n = NULL;
      current_nA = current_mA->next_n;
      current_nB = current_mB->next_n;
      current_n = NULL;
      while (current_nA != NULL || current_nB != NULL) {
         result_n = (n_node *) malloc(sizeof *result_n);
         n_of_A = (current_nA == NULL) ? N + 1 : current_nA->n;
         n_of_B = (current_nB == NULL) ? N + 1 : current_nB->n;
         if (n_of_A < n_of_B) {
            result_n->element = current_nA->element;
            result_n->n = current_nA->n ;
            current_nA = current_nA->next_n;
         } else if (n_of_A > n_of_B) { 
            result_n->element = current_nB->element;
            result_n->n = current_nB->n ;
            current_nB = current_nB->next_n;
         } else {
            result_n->element = current_nA->element + current_nB->element;
            result_n->n = current_nA->n ;       /* or current_nB->n */
            current_nA = current_nA->next_n;
            current_nB = current_nB->next_n;
         }
         if (current_n == NULL) {
            current_m->next_n = result_n;
         } else {
            current_n->next_n = result_n;
         }
         current_n = result_n;
         current_n->next_n = NULL;
      }
      current_mA = current_mA->next_m;
      current_mB = current_mB->next_m;
   }
   result_arr->no_of_rows = arrA->no_of_rows;    /* same a arrB */
   result_arr->no_of_cols = arrA->no_of_cols;    /* same a arrB */
   return result_arr;
}
/*-----------------------------------------------------------------------------*/
/* Function to print out the array in the classical representation from reading*/
/* the adjacency list. It also prints the element subscript to show that the   */
/* elements with zero value do not have a node in the adjacency list.          */
/*-----------------------------------------------------------------------------*/
void adjacency_to_classical(arr *arr_to_print, int classical_array[M][N]){
   int i, j;
   n_node *current_n;
   m_node *current_m;
   for(i=0;i<M;i++){for(j=0;j<N;j++){classical_array[i][j]=0;}}
   i=0;
   current_m = arr_to_print->m;
   while (current_m != NULL) {
      current_n = current_m->next_n;
      j=0;
      while (current_n != NULL){
         printf("[%i,%i]=%i  ",(i + 1) ,current_n->n, current_n->element);
         while((j+1)<current_n->n){
            classical_array[i][j++] = 0;
         }
         classical_array[i][j++] = current_n->element; 
         current_n = current_n->next_n;
      }
      i++;
      printf("\n");
      current_m = current_m->next_m;
   }
   return;
}
/*-----------------------------------------------------------------------------*/
/* Function that creates the adjacency list representation from the classical  */
/* array representation entered by the user.                                   */
/*-----------------------------------------------------------------------------*/
arr *classical_to_adjacency(int array_rep[M][N]){
   arr *new_arr;
   n_node *current_n, *next_n;
   m_node *current_m, *next_m;
   int n = 1, i, j;
   current_m = NULL;
   new_arr = (arr *) malloc(sizeof *new_arr);
   new_arr->no_of_rows = 0;
   new_arr->no_of_cols = 0;
   printf("Adjacency list representation :\n");
   for (i=0; i<M; i++) {
      next_m = (m_node *) malloc(sizeof *next_m);
      if (current_m == NULL) {
         new_arr->m = next_m;
      } else {
         current_m->next_m = next_m;
      }
      current_m = next_m;
      current_m->next_m = NULL;
      current_m->next_n = NULL;
      current_n = NULL;
      n = 1;
      (new_arr->no_of_rows)++;
      for (j=0; j<N; j++) {
         if (array_rep[i][j] != 0){
            next_n = (n_node *) malloc(sizeof *current_n);
            next_n->element = array_rep[i][j];
            next_n->n = n;
            if (current_n == NULL){
               current_m->next_n = next_n;
            } else {
               current_n->next_n = next_n;
            }
            current_n = next_n;
            current_n->next_n = NULL;
         }
         new_arr->no_of_cols = (new_arr->no_of_cols < n) ? n : new_arr->no_of_cols; 
         n++;
      }
   }
   return new_arr;
}
/*-----------------------------------------------------------------------------*/
/* Function to print the array onto the screen                                 */
/*-----------------------------------------------------------------------------*/
void  print_classical_array(int x[M][N]){
   int i, j;
   printf("Classical array representation:\n");
   for (i=0; i<M; i++){
      for (j=0; j<N; j++){
         printf(" %4i ",x[i][j]);
      }
      printf("\n");
   }
   return;
} 
/*-------------------------------  E N D  -------------------------------------*/
