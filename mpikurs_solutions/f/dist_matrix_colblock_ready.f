      subroutine dist_matrix_colblock( m, n, a, al )
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
      integer           npmax
      parameter         (npmax = 128)
c  FORTRAN MPI-INCLUDE-file
      include 'mpif.h'
c  local variables
      integer  ierr, nproc, myid, ip
      integer  firstind(0:npmax), counts(0:npmax),displs(0:npmax),cnt 

      call MPI_COMM_SIZE( MPI_COMM_WORLD, nproc, ierr )
      call MPI_COMM_RANK( MPI_COMM_WORLD, myid, ierr )

      call dist_index( n, firstind )
      cnt =  m * ( firstind(myid+1) - firstind(myid) )
      do ip = 0 , nproc-1
         displs(ip) = m * firstind(ip)
         counts(ip) = m * ( firstind(ip+1) - firstind(ip) )
      end do
      call MPI_SCATTERV(a, counts, displs, MPI_DOUBLE_PRECISION,
     :                  al, cnt, MPI_DOUBLE_PRECISION, 0,
     :                  MPI_COMM_WORLD, ierr )
      return
      end

