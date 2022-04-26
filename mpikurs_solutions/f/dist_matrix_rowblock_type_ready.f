      subroutine dist_matrix_rowblock( m, n, a, al )
c     ===============================
c  all variables should be explicitely declared !
      implicit none 
c
      integer           m, n
      real*8            a(*), al(*)
c
c  +-------------+----------------+------------------------------------+
c  |  parameter  |  input/output  |  description                       |
c  +-------------+----------------+------------------------------------+
c  |  m          |  input         |  number of rows of a and al        |
c  |  n          |  input         |  number of columns of a            |
c  |  a          |  input         |  global matrix                     |
c  |  al         |  output        |  local matrix, containing          |
c  |             |                |  nl columns of a                   |
c  +-------------+----------------+------------------------------------+
c  |                                                                   |
c  |  Purpose of this subroutine is to distribute a matrix a           |
c  |  over all processors                                              |
c  |                                                                   |
c  +-------------------------------------------------------------------+
c  |  ruler                                                            |
c  |5 7 10   15   20   25   30   35   40   45   50   55   60   65   70 | 
c  ++-+--+----+----+----+----+----+----+----+----+----+----+----+----+-+
c
c  parameter constants
      integer           nprocmax
      parameter         (nprocmax = 128)
c  FORTRAN MPI-INCLUDE-file
      include 'mpif.h'
c  local variables
      integer           ierr, nproc, myid
      integer           firstind(0:nprocmax), 
     :                  ic, ip, ia, ncom, nl, i, j
      real*8            tmp
      integer rowblock, req


      call MPI_COMM_SIZE( MPI_COMM_WORLD, nproc, ierr )
      call MPI_COMM_RANK( MPI_COMM_WORLD, myid, ierr )

      call dist_index( m, firstind )
         ncom = n * (firstind(myid+1) - firstind(myid))
         call MPI_IRECV( al, ncom, MPI_DOUBLE_PRECISION, 0, 0,
     :                     MPI_COMM_WORLD, req, ierr ) 
      
      if (myid.eq.0) then
         do ip = 0 , nproc-1
            nl = firstind(ip+1) - firstind(ip)
! define and commit new datatype rowblock
! syntax : MPI_TYPE_VECTOR(n, nl, n, MPI_DOUBLE_PRECISION, rowblock, ierr)

            call MPI_TYPE_VECTOR(n, nl, m, MPI_DOUBLE_PRECISION, 
     :                           rowblock, ierr)
            call MPI_TYPE_COMMIT(rowblock,ierr)
            ia = firstind(ip)+1
            call MPI_SEND( a(ia),1, rowblock, ip, 0,  
     :                     MPI_COMM_WORLD, ierr )
            call MPI_Type_free(rowblock,ierr)
         end do
      end if
      call MPI_WAIT( req, MPI_STATUS_IGNORE, ierr )


      return
      end

