#pragma once

namespace QPLUM
{
  	
#define MAX_ELEMENTS 1000000
  
  /** @class DenseMatrix
   * @brief For a matrix of m rows and n columns are going to assign a block of memory of size sizeof(A_Type) * m * n.
   * Since oldest_row_index_ corresponds to row 0,
   * the ith row and jth column would be the entry memblock [ ( ( i + oldest_row_index_ ) % num_rows_ ) * num_columns_ + j ]
   */

  template < typename A_Type >
  class DenseMatrix
  {
    protected:
      A_Type * memblock_ ; ///< the block of memory where we are storing the data
      unsigned int oldest_row_index_; ///< this is the index of the row that corresponds to row 0
      unsigned int num_rows_;
      unsigned int num_columns_;

    public:
      DenseMatrix ( )
          : memblock_ ( nullptr ),
            oldest_row_index_ ( 0u ),
            num_rows_ ( 0u ),
            num_columns_ ( 0u )
      {}
      
      DenseMatrix ( unsigned int num_rows, unsigned int num_columns )
        : memblock_ ( nullptr ),
          oldest_row_index_ ( 0u ),
          num_rows_ ( num_rows ),
          num_columns_ ( num_columns )
      {
        int num_elements = num_rows_ * num_columns_;
        if ( num_elements > MAX_ELEMENTS )
        {
          std::bad_alloc exception;
          throw exception;
        }
        memblock_ = new A_Type [ num_elements ]; // memory must be alllocated on the heap since we don't know how much to allocate at compile time.
        memset( memblock_, 0, num_elements * sizeof( A_Type ) );
      }
      
      ~DenseMatrix ( )
      {
        if ( nullptr != memblock_ )
        { 
          delete[] memblock_;
          memblock_ = nullptr;
        }
      }
      
      /** @brief Sets # rows and columns, and allocates memory 
       */
      void initialize ( unsigned int num_rows, unsigned int num_columns )
      {
        num_rows_ = num_rows ;
        num_columns_ = num_columns ;
        int num_elements = num_rows_ * num_columns_;
        if ( num_elements > MAX_ELEMENTS )
        {
          // TODO make our own set of exceptions
          std::bad_alloc exception;
          throw exception;
        }
        if ( nullptr == memblock_ )
        {
          memblock_ = new A_Type [ num_elements ]; // memory must be allocated on the heap since we don't know how much to allocate at compile time.
        }
        else
        {
          //if ( DebugLogger::GetUniqueInstance().CheckLoggingLevel( CDS_ERROR ) )
          //{
          //  DBGLOG_GET_FUNC_LINE << "CDS_ERROR : DenseMatrix: memory already allocated, why are you initiazing again" << DBGLOG_GET_ENDL_FLUSH ; 
          //}
        }
      }

      /** @brief This function is not expected to change the memblock pointer, 
       *  but it shoudl be allowed to change it's contents
       */
      inline A_Type & operator()( unsigned int row_index, unsigned int column_index )
      {
        /*if ( column_index >= num_columns_ )
        {
         if ( DebugLogger::GetUniqueInstance().CheckLoggingLevel( CDS_ERROR ) )
         {
           DBGLOG_GET_FUNC_LINE << "CDS_ERROR : Invalid column access in DenseMatrix" << DBGLOG_GET_ENDL_FLUSH ; 
         }
         exit( EXIT_FAILURE );
        }*/
        return memblock_ [ ( ( row_index + oldest_row_index_ ) % num_rows_ ) * num_columns_ + column_index ] ;
      }

      /** @brief This function is not expected to change the memblock pointer or contents, 
       * but it shoudl return the value requested ( by a copy constructor if needed )
       */
      inline A_Type operator()( unsigned int row_index, unsigned int column_index ) const
      {
        /*if ( column_index >= num_columns_ )
        {
         if ( DebugLogger::GetUniqueInstance().CheckLoggingLevel( CDS_ERROR ) )
         {
           DBGLOG_GET_FUNC_LINE << "CDS_ERROR : Invalid column access in DenseMatrix" << DBGLOG_GET_ENDL_FLUSH ; 
         }
         exit( EXIT_FAILURE );
        }*/
        return memblock_ [ ( ( row_index + oldest_row_index_ ) % num_rows_ ) * num_columns_ + column_index ] ;
      }
      
      /** @brief Getter for number of rows */
      inline unsigned int num_rows() const
      {
        return num_rows_;
      }

      /** @brief Getter for number of columns */
      inline unsigned int num_cols() const
      {
        return num_columns_;
      }

      /** @brief Copy an entire row of matrix into the memory block */
      inline void copy_row( unsigned int row_index, A_Type * row_ptr ) const
      {
        memcpy( row_ptr, memblock_ + ( ( ( row_index + oldest_row_index_ ) % num_rows_ ) * num_columns_ ), num_columns_ * sizeof( A_Type ) );
      }

      /** @brief This forgets the last row */
      inline void roll_right()
      {
        oldest_row_index_ = ( oldest_row_index_ + 1 ) % num_rows_;
      }

      /** @brief Sets the row to all zeroes  */
      inline void set_row_to_zero( unsigned int row_index )
      {
        memset( memblock_ + ( ( ( row_index + oldest_row_index_ ) % num_rows_ ) * num_columns_ ), 0, num_columns_ * sizeof( A_Type ) );  
      }
  };
}
