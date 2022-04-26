      program hello
      implicit none
      include 'mpif.h'
      integer nproc, myid, version, subversion, ierr, pnamelen
      character*(40) pname
c---------------------------------------------------------- 
c        start MPI
c----------------------------------------------------------
      call MPI_INIT( ierr )
      call MPI_COMM_SIZE( MPI_COMM_WORLD, nproc, ierr )
      call MPI_COMM_RANK( MPI_COMM_WORLD, myid, ierr )
      call MPI_GET_PROCESSOR_NAME( pname, pnamelen, ierr ) 
      write(6,*)nproc, myid, pname
      if (myid.eq.0) then
        call MPI_GET_VERSION( version, subversion, ierr )
        write(6,*) 'version: ',version,'subversion:',subversion
      end if

      call MPI_FINALIZE (ierr)

      stop
      end






















