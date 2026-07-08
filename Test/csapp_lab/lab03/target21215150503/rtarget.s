
rtarget:     file format elf64-x86-64


Disassembly of section .init:

0000000000400c48 <_init>:
  400c48:	48 83 ec 08          	sub    $0x8,%rsp
  400c4c:	48 8b 05 a5 43 20 00 	mov    0x2043a5(%rip),%rax        # 604ff8 <__gmon_start__>
  400c53:	48 85 c0             	test   %rax,%rax
  400c56:	74 02                	je     400c5a <_init+0x12>
  400c58:	ff d0                	callq  *%rax
  400c5a:	48 83 c4 08          	add    $0x8,%rsp
  400c5e:	c3                   	retq   

Disassembly of section .plt:

0000000000400c60 <.plt>:
  400c60:	ff 35 a2 43 20 00    	pushq  0x2043a2(%rip)        # 605008 <_GLOBAL_OFFSET_TABLE_+0x8>
  400c66:	ff 25 a4 43 20 00    	jmpq   *0x2043a4(%rip)        # 605010 <_GLOBAL_OFFSET_TABLE_+0x10>
  400c6c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000400c70 <strcasecmp@plt>:
  400c70:	ff 25 a2 43 20 00    	jmpq   *0x2043a2(%rip)        # 605018 <strcasecmp@GLIBC_2.2.5>
  400c76:	68 00 00 00 00       	pushq  $0x0
  400c7b:	e9 e0 ff ff ff       	jmpq   400c60 <.plt>

0000000000400c80 <__errno_location@plt>:
  400c80:	ff 25 9a 43 20 00    	jmpq   *0x20439a(%rip)        # 605020 <__errno_location@GLIBC_2.2.5>
  400c86:	68 01 00 00 00       	pushq  $0x1
  400c8b:	e9 d0 ff ff ff       	jmpq   400c60 <.plt>

0000000000400c90 <srandom@plt>:
  400c90:	ff 25 92 43 20 00    	jmpq   *0x204392(%rip)        # 605028 <srandom@GLIBC_2.2.5>
  400c96:	68 02 00 00 00       	pushq  $0x2
  400c9b:	e9 c0 ff ff ff       	jmpq   400c60 <.plt>

0000000000400ca0 <strncmp@plt>:
  400ca0:	ff 25 8a 43 20 00    	jmpq   *0x20438a(%rip)        # 605030 <strncmp@GLIBC_2.2.5>
  400ca6:	68 03 00 00 00       	pushq  $0x3
  400cab:	e9 b0 ff ff ff       	jmpq   400c60 <.plt>

0000000000400cb0 <strcpy@plt>:
  400cb0:	ff 25 82 43 20 00    	jmpq   *0x204382(%rip)        # 605038 <strcpy@GLIBC_2.2.5>
  400cb6:	68 04 00 00 00       	pushq  $0x4
  400cbb:	e9 a0 ff ff ff       	jmpq   400c60 <.plt>

0000000000400cc0 <puts@plt>:
  400cc0:	ff 25 7a 43 20 00    	jmpq   *0x20437a(%rip)        # 605040 <puts@GLIBC_2.2.5>
  400cc6:	68 05 00 00 00       	pushq  $0x5
  400ccb:	e9 90 ff ff ff       	jmpq   400c60 <.plt>

0000000000400cd0 <write@plt>:
  400cd0:	ff 25 72 43 20 00    	jmpq   *0x204372(%rip)        # 605048 <write@GLIBC_2.2.5>
  400cd6:	68 06 00 00 00       	pushq  $0x6
  400cdb:	e9 80 ff ff ff       	jmpq   400c60 <.plt>

0000000000400ce0 <__stack_chk_fail@plt>:
  400ce0:	ff 25 6a 43 20 00    	jmpq   *0x20436a(%rip)        # 605050 <__stack_chk_fail@GLIBC_2.4>
  400ce6:	68 07 00 00 00       	pushq  $0x7
  400ceb:	e9 70 ff ff ff       	jmpq   400c60 <.plt>

0000000000400cf0 <mmap@plt>:
  400cf0:	ff 25 62 43 20 00    	jmpq   *0x204362(%rip)        # 605058 <mmap@GLIBC_2.2.5>
  400cf6:	68 08 00 00 00       	pushq  $0x8
  400cfb:	e9 60 ff ff ff       	jmpq   400c60 <.plt>

0000000000400d00 <memset@plt>:
  400d00:	ff 25 5a 43 20 00    	jmpq   *0x20435a(%rip)        # 605060 <memset@GLIBC_2.2.5>
  400d06:	68 09 00 00 00       	pushq  $0x9
  400d0b:	e9 50 ff ff ff       	jmpq   400c60 <.plt>

0000000000400d10 <alarm@plt>:
  400d10:	ff 25 52 43 20 00    	jmpq   *0x204352(%rip)        # 605068 <alarm@GLIBC_2.2.5>
  400d16:	68 0a 00 00 00       	pushq  $0xa
  400d1b:	e9 40 ff ff ff       	jmpq   400c60 <.plt>

0000000000400d20 <close@plt>:
  400d20:	ff 25 4a 43 20 00    	jmpq   *0x20434a(%rip)        # 605070 <close@GLIBC_2.2.5>
  400d26:	68 0b 00 00 00       	pushq  $0xb
  400d2b:	e9 30 ff ff ff       	jmpq   400c60 <.plt>

0000000000400d30 <read@plt>:
  400d30:	ff 25 42 43 20 00    	jmpq   *0x204342(%rip)        # 605078 <read@GLIBC_2.2.5>
  400d36:	68 0c 00 00 00       	pushq  $0xc
  400d3b:	e9 20 ff ff ff       	jmpq   400c60 <.plt>

0000000000400d40 <signal@plt>:
  400d40:	ff 25 3a 43 20 00    	jmpq   *0x20433a(%rip)        # 605080 <signal@GLIBC_2.2.5>
  400d46:	68 0d 00 00 00       	pushq  $0xd
  400d4b:	e9 10 ff ff ff       	jmpq   400c60 <.plt>

0000000000400d50 <gethostbyname@plt>:
  400d50:	ff 25 32 43 20 00    	jmpq   *0x204332(%rip)        # 605088 <gethostbyname@GLIBC_2.2.5>
  400d56:	68 0e 00 00 00       	pushq  $0xe
  400d5b:	e9 00 ff ff ff       	jmpq   400c60 <.plt>

0000000000400d60 <__memmove_chk@plt>:
  400d60:	ff 25 2a 43 20 00    	jmpq   *0x20432a(%rip)        # 605090 <__memmove_chk@GLIBC_2.3.4>
  400d66:	68 0f 00 00 00       	pushq  $0xf
  400d6b:	e9 f0 fe ff ff       	jmpq   400c60 <.plt>

0000000000400d70 <strtol@plt>:
  400d70:	ff 25 22 43 20 00    	jmpq   *0x204322(%rip)        # 605098 <strtol@GLIBC_2.2.5>
  400d76:	68 10 00 00 00       	pushq  $0x10
  400d7b:	e9 e0 fe ff ff       	jmpq   400c60 <.plt>

0000000000400d80 <memcpy@plt>:
  400d80:	ff 25 1a 43 20 00    	jmpq   *0x20431a(%rip)        # 6050a0 <memcpy@GLIBC_2.14>
  400d86:	68 11 00 00 00       	pushq  $0x11
  400d8b:	e9 d0 fe ff ff       	jmpq   400c60 <.plt>

0000000000400d90 <time@plt>:
  400d90:	ff 25 12 43 20 00    	jmpq   *0x204312(%rip)        # 6050a8 <time@GLIBC_2.2.5>
  400d96:	68 12 00 00 00       	pushq  $0x12
  400d9b:	e9 c0 fe ff ff       	jmpq   400c60 <.plt>

0000000000400da0 <random@plt>:
  400da0:	ff 25 0a 43 20 00    	jmpq   *0x20430a(%rip)        # 6050b0 <random@GLIBC_2.2.5>
  400da6:	68 13 00 00 00       	pushq  $0x13
  400dab:	e9 b0 fe ff ff       	jmpq   400c60 <.plt>

0000000000400db0 <_IO_getc@plt>:
  400db0:	ff 25 02 43 20 00    	jmpq   *0x204302(%rip)        # 6050b8 <_IO_getc@GLIBC_2.2.5>
  400db6:	68 14 00 00 00       	pushq  $0x14
  400dbb:	e9 a0 fe ff ff       	jmpq   400c60 <.plt>

0000000000400dc0 <__isoc99_sscanf@plt>:
  400dc0:	ff 25 fa 42 20 00    	jmpq   *0x2042fa(%rip)        # 6050c0 <__isoc99_sscanf@GLIBC_2.7>
  400dc6:	68 15 00 00 00       	pushq  $0x15
  400dcb:	e9 90 fe ff ff       	jmpq   400c60 <.plt>

0000000000400dd0 <munmap@plt>:
  400dd0:	ff 25 f2 42 20 00    	jmpq   *0x2042f2(%rip)        # 6050c8 <munmap@GLIBC_2.2.5>
  400dd6:	68 16 00 00 00       	pushq  $0x16
  400ddb:	e9 80 fe ff ff       	jmpq   400c60 <.plt>

0000000000400de0 <__printf_chk@plt>:
  400de0:	ff 25 ea 42 20 00    	jmpq   *0x2042ea(%rip)        # 6050d0 <__printf_chk@GLIBC_2.3.4>
  400de6:	68 17 00 00 00       	pushq  $0x17
  400deb:	e9 70 fe ff ff       	jmpq   400c60 <.plt>

0000000000400df0 <fopen@plt>:
  400df0:	ff 25 e2 42 20 00    	jmpq   *0x2042e2(%rip)        # 6050d8 <fopen@GLIBC_2.2.5>
  400df6:	68 18 00 00 00       	pushq  $0x18
  400dfb:	e9 60 fe ff ff       	jmpq   400c60 <.plt>

0000000000400e00 <getopt@plt>:
  400e00:	ff 25 da 42 20 00    	jmpq   *0x2042da(%rip)        # 6050e0 <getopt@GLIBC_2.2.5>
  400e06:	68 19 00 00 00       	pushq  $0x19
  400e0b:	e9 50 fe ff ff       	jmpq   400c60 <.plt>

0000000000400e10 <strtoul@plt>:
  400e10:	ff 25 d2 42 20 00    	jmpq   *0x2042d2(%rip)        # 6050e8 <strtoul@GLIBC_2.2.5>
  400e16:	68 1a 00 00 00       	pushq  $0x1a
  400e1b:	e9 40 fe ff ff       	jmpq   400c60 <.plt>

0000000000400e20 <gethostname@plt>:
  400e20:	ff 25 ca 42 20 00    	jmpq   *0x2042ca(%rip)        # 6050f0 <gethostname@GLIBC_2.2.5>
  400e26:	68 1b 00 00 00       	pushq  $0x1b
  400e2b:	e9 30 fe ff ff       	jmpq   400c60 <.plt>

0000000000400e30 <exit@plt>:
  400e30:	ff 25 c2 42 20 00    	jmpq   *0x2042c2(%rip)        # 6050f8 <exit@GLIBC_2.2.5>
  400e36:	68 1c 00 00 00       	pushq  $0x1c
  400e3b:	e9 20 fe ff ff       	jmpq   400c60 <.plt>

0000000000400e40 <connect@plt>:
  400e40:	ff 25 ba 42 20 00    	jmpq   *0x2042ba(%rip)        # 605100 <connect@GLIBC_2.2.5>
  400e46:	68 1d 00 00 00       	pushq  $0x1d
  400e4b:	e9 10 fe ff ff       	jmpq   400c60 <.plt>

0000000000400e50 <__fprintf_chk@plt>:
  400e50:	ff 25 b2 42 20 00    	jmpq   *0x2042b2(%rip)        # 605108 <__fprintf_chk@GLIBC_2.3.4>
  400e56:	68 1e 00 00 00       	pushq  $0x1e
  400e5b:	e9 00 fe ff ff       	jmpq   400c60 <.plt>

0000000000400e60 <__sprintf_chk@plt>:
  400e60:	ff 25 aa 42 20 00    	jmpq   *0x2042aa(%rip)        # 605110 <__sprintf_chk@GLIBC_2.3.4>
  400e66:	68 1f 00 00 00       	pushq  $0x1f
  400e6b:	e9 f0 fd ff ff       	jmpq   400c60 <.plt>

0000000000400e70 <socket@plt>:
  400e70:	ff 25 a2 42 20 00    	jmpq   *0x2042a2(%rip)        # 605118 <socket@GLIBC_2.2.5>
  400e76:	68 20 00 00 00       	pushq  $0x20
  400e7b:	e9 e0 fd ff ff       	jmpq   400c60 <.plt>

Disassembly of section .text:

0000000000400e80 <_start>:
  400e80:	31 ed                	xor    %ebp,%ebp
  400e82:	49 89 d1             	mov    %rdx,%r9
  400e85:	5e                   	pop    %rsi
  400e86:	48 89 e2             	mov    %rsp,%rdx
  400e89:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
  400e8d:	50                   	push   %rax
  400e8e:	54                   	push   %rsp
  400e8f:	49 c7 c0 b0 30 40 00 	mov    $0x4030b0,%r8
  400e96:	48 c7 c1 40 30 40 00 	mov    $0x403040,%rcx
  400e9d:	48 c7 c7 8e 11 40 00 	mov    $0x40118e,%rdi
  400ea4:	ff 15 46 41 20 00    	callq  *0x204146(%rip)        # 604ff0 <__libc_start_main@GLIBC_2.2.5>
  400eaa:	f4                   	hlt    
  400eab:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000400eb0 <_dl_relocate_static_pie>:
  400eb0:	f3 c3                	repz retq 
  400eb2:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  400eb9:	00 00 00 
  400ebc:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000400ec0 <deregister_tm_clones>:
  400ec0:	55                   	push   %rbp
  400ec1:	b8 90 54 60 00       	mov    $0x605490,%eax
  400ec6:	48 3d 90 54 60 00    	cmp    $0x605490,%rax
  400ecc:	48 89 e5             	mov    %rsp,%rbp
  400ecf:	74 17                	je     400ee8 <deregister_tm_clones+0x28>
  400ed1:	b8 00 00 00 00       	mov    $0x0,%eax
  400ed6:	48 85 c0             	test   %rax,%rax
  400ed9:	74 0d                	je     400ee8 <deregister_tm_clones+0x28>
  400edb:	5d                   	pop    %rbp
  400edc:	bf 90 54 60 00       	mov    $0x605490,%edi
  400ee1:	ff e0                	jmpq   *%rax
  400ee3:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
  400ee8:	5d                   	pop    %rbp
  400ee9:	c3                   	retq   
  400eea:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000400ef0 <register_tm_clones>:
  400ef0:	be 90 54 60 00       	mov    $0x605490,%esi
  400ef5:	55                   	push   %rbp
  400ef6:	48 81 ee 90 54 60 00 	sub    $0x605490,%rsi
  400efd:	48 89 e5             	mov    %rsp,%rbp
  400f00:	48 c1 fe 03          	sar    $0x3,%rsi
  400f04:	48 89 f0             	mov    %rsi,%rax
  400f07:	48 c1 e8 3f          	shr    $0x3f,%rax
  400f0b:	48 01 c6             	add    %rax,%rsi
  400f0e:	48 d1 fe             	sar    %rsi
  400f11:	74 15                	je     400f28 <register_tm_clones+0x38>
  400f13:	b8 00 00 00 00       	mov    $0x0,%eax
  400f18:	48 85 c0             	test   %rax,%rax
  400f1b:	74 0b                	je     400f28 <register_tm_clones+0x38>
  400f1d:	5d                   	pop    %rbp
  400f1e:	bf 90 54 60 00       	mov    $0x605490,%edi
  400f23:	ff e0                	jmpq   *%rax
  400f25:	0f 1f 00             	nopl   (%rax)
  400f28:	5d                   	pop    %rbp
  400f29:	c3                   	retq   
  400f2a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000400f30 <__do_global_dtors_aux>:
  400f30:	80 3d 91 45 20 00 00 	cmpb   $0x0,0x204591(%rip)        # 6054c8 <completed.7698>
  400f37:	75 17                	jne    400f50 <__do_global_dtors_aux+0x20>
  400f39:	55                   	push   %rbp
  400f3a:	48 89 e5             	mov    %rsp,%rbp
  400f3d:	e8 7e ff ff ff       	callq  400ec0 <deregister_tm_clones>
  400f42:	c6 05 7f 45 20 00 01 	movb   $0x1,0x20457f(%rip)        # 6054c8 <completed.7698>
  400f49:	5d                   	pop    %rbp
  400f4a:	c3                   	retq   
  400f4b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
  400f50:	f3 c3                	repz retq 
  400f52:	0f 1f 40 00          	nopl   0x0(%rax)
  400f56:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  400f5d:	00 00 00 

0000000000400f60 <frame_dummy>:
  400f60:	55                   	push   %rbp
  400f61:	48 89 e5             	mov    %rsp,%rbp
  400f64:	5d                   	pop    %rbp
  400f65:	eb 89                	jmp    400ef0 <register_tm_clones>

0000000000400f67 <usage>:
  400f67:	48 83 ec 08          	sub    $0x8,%rsp
  400f6b:	48 89 fa             	mov    %rdi,%rdx
  400f6e:	83 3d 93 45 20 00 00 	cmpl   $0x0,0x204593(%rip)        # 605508 <is_checker>
  400f75:	74 50                	je     400fc7 <usage+0x60>
  400f77:	48 8d 35 4a 21 00 00 	lea    0x214a(%rip),%rsi        # 4030c8 <_IO_stdin_used+0x8>
  400f7e:	bf 01 00 00 00       	mov    $0x1,%edi
  400f83:	b8 00 00 00 00       	mov    $0x0,%eax
  400f88:	e8 53 fe ff ff       	callq  400de0 <__printf_chk@plt>
  400f8d:	48 8d 3d 6c 21 00 00 	lea    0x216c(%rip),%rdi        # 403100 <_IO_stdin_used+0x40>
  400f94:	e8 27 fd ff ff       	callq  400cc0 <puts@plt>
  400f99:	48 8d 3d d8 22 00 00 	lea    0x22d8(%rip),%rdi        # 403278 <_IO_stdin_used+0x1b8>
  400fa0:	e8 1b fd ff ff       	callq  400cc0 <puts@plt>
  400fa5:	48 8d 3d 7c 21 00 00 	lea    0x217c(%rip),%rdi        # 403128 <_IO_stdin_used+0x68>
  400fac:	e8 0f fd ff ff       	callq  400cc0 <puts@plt>
  400fb1:	48 8d 3d da 22 00 00 	lea    0x22da(%rip),%rdi        # 403292 <_IO_stdin_used+0x1d2>
  400fb8:	e8 03 fd ff ff       	callq  400cc0 <puts@plt>
  400fbd:	bf 00 00 00 00       	mov    $0x0,%edi
  400fc2:	e8 69 fe ff ff       	callq  400e30 <exit@plt>
  400fc7:	48 8d 35 e0 22 00 00 	lea    0x22e0(%rip),%rsi        # 4032ae <_IO_stdin_used+0x1ee>
  400fce:	bf 01 00 00 00       	mov    $0x1,%edi
  400fd3:	b8 00 00 00 00       	mov    $0x0,%eax
  400fd8:	e8 03 fe ff ff       	callq  400de0 <__printf_chk@plt>
  400fdd:	48 8d 3d 6c 21 00 00 	lea    0x216c(%rip),%rdi        # 403150 <_IO_stdin_used+0x90>
  400fe4:	e8 d7 fc ff ff       	callq  400cc0 <puts@plt>
  400fe9:	48 8d 3d 88 21 00 00 	lea    0x2188(%rip),%rdi        # 403178 <_IO_stdin_used+0xb8>
  400ff0:	e8 cb fc ff ff       	callq  400cc0 <puts@plt>
  400ff5:	48 8d 3d d0 22 00 00 	lea    0x22d0(%rip),%rdi        # 4032cc <_IO_stdin_used+0x20c>
  400ffc:	e8 bf fc ff ff       	callq  400cc0 <puts@plt>
  401001:	eb ba                	jmp    400fbd <usage+0x56>

0000000000401003 <initialize_target>:
  401003:	55                   	push   %rbp
  401004:	53                   	push   %rbx
  401005:	48 81 ec 18 21 00 00 	sub    $0x2118,%rsp
  40100c:	89 f5                	mov    %esi,%ebp
  40100e:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  401015:	00 00 
  401017:	48 89 84 24 08 21 00 	mov    %rax,0x2108(%rsp)
  40101e:	00 
  40101f:	31 c0                	xor    %eax,%eax
  401021:	89 3d d1 44 20 00    	mov    %edi,0x2044d1(%rip)        # 6054f8 <check_level>
  401027:	8b 3d 03 41 20 00    	mov    0x204103(%rip),%edi        # 605130 <target_id>
  40102d:	e8 eb 1f 00 00       	callq  40301d <gencookie>
  401032:	89 05 cc 44 20 00    	mov    %eax,0x2044cc(%rip)        # 605504 <cookie>
  401038:	89 c7                	mov    %eax,%edi
  40103a:	e8 de 1f 00 00       	callq  40301d <gencookie>
  40103f:	89 05 bb 44 20 00    	mov    %eax,0x2044bb(%rip)        # 605500 <authkey>
  401045:	8b 05 e5 40 20 00    	mov    0x2040e5(%rip),%eax        # 605130 <target_id>
  40104b:	8d 78 01             	lea    0x1(%rax),%edi
  40104e:	e8 3d fc ff ff       	callq  400c90 <srandom@plt>
  401053:	e8 48 fd ff ff       	callq  400da0 <random@plt>
  401058:	89 c7                	mov    %eax,%edi
  40105a:	e8 1a 03 00 00       	callq  401379 <scramble>
  40105f:	89 c3                	mov    %eax,%ebx
  401061:	85 ed                	test   %ebp,%ebp
  401063:	75 54                	jne    4010b9 <initialize_target+0xb6>
  401065:	b8 00 00 00 00       	mov    $0x0,%eax
  40106a:	01 d8                	add    %ebx,%eax
  40106c:	0f b7 c0             	movzwl %ax,%eax
  40106f:	8d 04 c5 00 01 00 00 	lea    0x100(,%rax,8),%eax
  401076:	89 c0                	mov    %eax,%eax
  401078:	48 89 05 01 44 20 00 	mov    %rax,0x204401(%rip)        # 605480 <buf_offset>
  40107f:	c6 05 a2 50 20 00 72 	movb   $0x72,0x2050a2(%rip)        # 606128 <target_prefix>
  401086:	83 3d fb 43 20 00 00 	cmpl   $0x0,0x2043fb(%rip)        # 605488 <notify>
  40108d:	74 09                	je     401098 <initialize_target+0x95>
  40108f:	83 3d 72 44 20 00 00 	cmpl   $0x0,0x204472(%rip)        # 605508 <is_checker>
  401096:	74 39                	je     4010d1 <initialize_target+0xce>
  401098:	48 8b 84 24 08 21 00 	mov    0x2108(%rsp),%rax
  40109f:	00 
  4010a0:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
  4010a7:	00 00 
  4010a9:	0f 85 da 00 00 00    	jne    401189 <initialize_target+0x186>
  4010af:	48 81 c4 18 21 00 00 	add    $0x2118,%rsp
  4010b6:	5b                   	pop    %rbx
  4010b7:	5d                   	pop    %rbp
  4010b8:	c3                   	retq   
  4010b9:	bf 00 00 00 00       	mov    $0x0,%edi
  4010be:	e8 cd fc ff ff       	callq  400d90 <time@plt>
  4010c3:	89 c7                	mov    %eax,%edi
  4010c5:	e8 c6 fb ff ff       	callq  400c90 <srandom@plt>
  4010ca:	e8 d1 fc ff ff       	callq  400da0 <random@plt>
  4010cf:	eb 99                	jmp    40106a <initialize_target+0x67>
  4010d1:	48 89 e7             	mov    %rsp,%rdi
  4010d4:	be 00 01 00 00       	mov    $0x100,%esi
  4010d9:	e8 42 fd ff ff       	callq  400e20 <gethostname@plt>
  4010de:	89 c5                	mov    %eax,%ebp
  4010e0:	85 c0                	test   %eax,%eax
  4010e2:	75 26                	jne    40110a <initialize_target+0x107>
  4010e4:	89 c3                	mov    %eax,%ebx
  4010e6:	48 63 c3             	movslq %ebx,%rax
  4010e9:	48 8d 15 70 40 20 00 	lea    0x204070(%rip),%rdx        # 605160 <host_table>
  4010f0:	48 8b 3c c2          	mov    (%rdx,%rax,8),%rdi
  4010f4:	48 85 ff             	test   %rdi,%rdi
  4010f7:	74 2c                	je     401125 <initialize_target+0x122>
  4010f9:	48 89 e6             	mov    %rsp,%rsi
  4010fc:	e8 6f fb ff ff       	callq  400c70 <strcasecmp@plt>
  401101:	85 c0                	test   %eax,%eax
  401103:	74 1b                	je     401120 <initialize_target+0x11d>
  401105:	83 c3 01             	add    $0x1,%ebx
  401108:	eb dc                	jmp    4010e6 <initialize_target+0xe3>
  40110a:	48 8d 3d 97 20 00 00 	lea    0x2097(%rip),%rdi        # 4031a8 <_IO_stdin_used+0xe8>
  401111:	e8 aa fb ff ff       	callq  400cc0 <puts@plt>
  401116:	bf 08 00 00 00       	mov    $0x8,%edi
  40111b:	e8 10 fd ff ff       	callq  400e30 <exit@plt>
  401120:	bd 01 00 00 00       	mov    $0x1,%ebp
  401125:	85 ed                	test   %ebp,%ebp
  401127:	74 3d                	je     401166 <initialize_target+0x163>
  401129:	48 8d bc 24 00 01 00 	lea    0x100(%rsp),%rdi
  401130:	00 
  401131:	e8 17 1c 00 00       	callq  402d4d <init_driver>
  401136:	85 c0                	test   %eax,%eax
  401138:	0f 89 5a ff ff ff    	jns    401098 <initialize_target+0x95>
  40113e:	48 8d 94 24 00 01 00 	lea    0x100(%rsp),%rdx
  401145:	00 
  401146:	48 8d 35 d3 20 00 00 	lea    0x20d3(%rip),%rsi        # 403220 <_IO_stdin_used+0x160>
  40114d:	bf 01 00 00 00       	mov    $0x1,%edi
  401152:	b8 00 00 00 00       	mov    $0x0,%eax
  401157:	e8 84 fc ff ff       	callq  400de0 <__printf_chk@plt>
  40115c:	bf 08 00 00 00       	mov    $0x8,%edi
  401161:	e8 ca fc ff ff       	callq  400e30 <exit@plt>
  401166:	48 89 e2             	mov    %rsp,%rdx
  401169:	48 8d 35 70 20 00 00 	lea    0x2070(%rip),%rsi        # 4031e0 <_IO_stdin_used+0x120>
  401170:	bf 01 00 00 00       	mov    $0x1,%edi
  401175:	b8 00 00 00 00       	mov    $0x0,%eax
  40117a:	e8 61 fc ff ff       	callq  400de0 <__printf_chk@plt>
  40117f:	bf 08 00 00 00       	mov    $0x8,%edi
  401184:	e8 a7 fc ff ff       	callq  400e30 <exit@plt>
  401189:	e8 52 fb ff ff       	callq  400ce0 <__stack_chk_fail@plt>

000000000040118e <main>:
  40118e:	41 56                	push   %r14
  401190:	41 55                	push   %r13
  401192:	41 54                	push   %r12
  401194:	55                   	push   %rbp
  401195:	53                   	push   %rbx
  401196:	41 89 fc             	mov    %edi,%r12d
  401199:	48 89 f3             	mov    %rsi,%rbx
  40119c:	48 c7 c6 54 20 40 00 	mov    $0x402054,%rsi
  4011a3:	bf 0b 00 00 00       	mov    $0xb,%edi
  4011a8:	e8 93 fb ff ff       	callq  400d40 <signal@plt>
  4011ad:	48 c7 c6 00 20 40 00 	mov    $0x402000,%rsi
  4011b4:	bf 07 00 00 00       	mov    $0x7,%edi
  4011b9:	e8 82 fb ff ff       	callq  400d40 <signal@plt>
  4011be:	48 c7 c6 a8 20 40 00 	mov    $0x4020a8,%rsi
  4011c5:	bf 04 00 00 00       	mov    $0x4,%edi
  4011ca:	e8 71 fb ff ff       	callq  400d40 <signal@plt>
  4011cf:	83 3d 32 43 20 00 00 	cmpl   $0x0,0x204332(%rip)        # 605508 <is_checker>
  4011d6:	75 26                	jne    4011fe <main+0x70>
  4011d8:	48 8d 2d 06 21 00 00 	lea    0x2106(%rip),%rbp        # 4032e5 <_IO_stdin_used+0x225>
  4011df:	48 8b 05 ba 42 20 00 	mov    0x2042ba(%rip),%rax        # 6054a0 <stdin@@GLIBC_2.2.5>
  4011e6:	48 89 05 03 43 20 00 	mov    %rax,0x204303(%rip)        # 6054f0 <infile>
  4011ed:	41 bd 00 00 00 00    	mov    $0x0,%r13d
  4011f3:	41 be 00 00 00 00    	mov    $0x0,%r14d
  4011f9:	e9 8d 00 00 00       	jmpq   40128b <main+0xfd>
  4011fe:	48 c7 c6 fc 20 40 00 	mov    $0x4020fc,%rsi
  401205:	bf 0e 00 00 00       	mov    $0xe,%edi
  40120a:	e8 31 fb ff ff       	callq  400d40 <signal@plt>
  40120f:	bf 05 00 00 00       	mov    $0x5,%edi
  401214:	e8 f7 fa ff ff       	callq  400d10 <alarm@plt>
  401219:	48 8d 2d ca 20 00 00 	lea    0x20ca(%rip),%rbp        # 4032ea <_IO_stdin_used+0x22a>
  401220:	eb bd                	jmp    4011df <main+0x51>
  401222:	48 8b 3b             	mov    (%rbx),%rdi
  401225:	e8 3d fd ff ff       	callq  400f67 <usage>
  40122a:	48 8d 35 44 23 00 00 	lea    0x2344(%rip),%rsi        # 403575 <_IO_stdin_used+0x4b5>
  401231:	48 8b 3d 70 42 20 00 	mov    0x204270(%rip),%rdi        # 6054a8 <optarg@@GLIBC_2.2.5>
  401238:	e8 b3 fb ff ff       	callq  400df0 <fopen@plt>
  40123d:	48 89 05 ac 42 20 00 	mov    %rax,0x2042ac(%rip)        # 6054f0 <infile>
  401244:	48 85 c0             	test   %rax,%rax
  401247:	75 42                	jne    40128b <main+0xfd>
  401249:	48 8b 0d 58 42 20 00 	mov    0x204258(%rip),%rcx        # 6054a8 <optarg@@GLIBC_2.2.5>
  401250:	48 8d 15 9b 20 00 00 	lea    0x209b(%rip),%rdx        # 4032f2 <_IO_stdin_used+0x232>
  401257:	be 01 00 00 00       	mov    $0x1,%esi
  40125c:	48 8b 3d 5d 42 20 00 	mov    0x20425d(%rip),%rdi        # 6054c0 <stderr@@GLIBC_2.2.5>
  401263:	e8 e8 fb ff ff       	callq  400e50 <__fprintf_chk@plt>
  401268:	b8 01 00 00 00       	mov    $0x1,%eax
  40126d:	e9 d9 00 00 00       	jmpq   40134b <main+0x1bd>
  401272:	ba 10 00 00 00       	mov    $0x10,%edx
  401277:	be 00 00 00 00       	mov    $0x0,%esi
  40127c:	48 8b 3d 25 42 20 00 	mov    0x204225(%rip),%rdi        # 6054a8 <optarg@@GLIBC_2.2.5>
  401283:	e8 88 fb ff ff       	callq  400e10 <strtoul@plt>
  401288:	41 89 c6             	mov    %eax,%r14d
  40128b:	48 89 ea             	mov    %rbp,%rdx
  40128e:	48 89 de             	mov    %rbx,%rsi
  401291:	44 89 e7             	mov    %r12d,%edi
  401294:	e8 67 fb ff ff       	callq  400e00 <getopt@plt>
  401299:	3c ff                	cmp    $0xff,%al
  40129b:	74 62                	je     4012ff <main+0x171>
  40129d:	0f be d0             	movsbl %al,%edx
  4012a0:	83 e8 61             	sub    $0x61,%eax
  4012a3:	3c 10                	cmp    $0x10,%al
  4012a5:	77 3a                	ja     4012e1 <main+0x153>
  4012a7:	0f b6 c0             	movzbl %al,%eax
  4012aa:	48 8d 0d 7f 20 00 00 	lea    0x207f(%rip),%rcx        # 403330 <_IO_stdin_used+0x270>
  4012b1:	48 63 04 81          	movslq (%rcx,%rax,4),%rax
  4012b5:	48 01 c8             	add    %rcx,%rax
  4012b8:	ff e0                	jmpq   *%rax
  4012ba:	ba 0a 00 00 00       	mov    $0xa,%edx
  4012bf:	be 00 00 00 00       	mov    $0x0,%esi
  4012c4:	48 8b 3d dd 41 20 00 	mov    0x2041dd(%rip),%rdi        # 6054a8 <optarg@@GLIBC_2.2.5>
  4012cb:	e8 a0 fa ff ff       	callq  400d70 <strtol@plt>
  4012d0:	41 89 c5             	mov    %eax,%r13d
  4012d3:	eb b6                	jmp    40128b <main+0xfd>
  4012d5:	c7 05 a9 41 20 00 00 	movl   $0x0,0x2041a9(%rip)        # 605488 <notify>
  4012dc:	00 00 00 
  4012df:	eb aa                	jmp    40128b <main+0xfd>
  4012e1:	48 8d 35 27 20 00 00 	lea    0x2027(%rip),%rsi        # 40330f <_IO_stdin_used+0x24f>
  4012e8:	bf 01 00 00 00       	mov    $0x1,%edi
  4012ed:	b8 00 00 00 00       	mov    $0x0,%eax
  4012f2:	e8 e9 fa ff ff       	callq  400de0 <__printf_chk@plt>
  4012f7:	48 8b 3b             	mov    (%rbx),%rdi
  4012fa:	e8 68 fc ff ff       	callq  400f67 <usage>
  4012ff:	be 01 00 00 00       	mov    $0x1,%esi
  401304:	44 89 ef             	mov    %r13d,%edi
  401307:	e8 f7 fc ff ff       	callq  401003 <initialize_target>
  40130c:	83 3d f5 41 20 00 00 	cmpl   $0x0,0x2041f5(%rip)        # 605508 <is_checker>
  401313:	74 09                	je     40131e <main+0x190>
  401315:	44 39 35 e4 41 20 00 	cmp    %r14d,0x2041e4(%rip)        # 605500 <authkey>
  40131c:	75 36                	jne    401354 <main+0x1c6>
  40131e:	8b 15 e0 41 20 00    	mov    0x2041e0(%rip),%edx        # 605504 <cookie>
  401324:	48 8d 35 f7 1f 00 00 	lea    0x1ff7(%rip),%rsi        # 403322 <_IO_stdin_used+0x262>
  40132b:	bf 01 00 00 00       	mov    $0x1,%edi
  401330:	b8 00 00 00 00       	mov    $0x0,%eax
  401335:	e8 a6 fa ff ff       	callq  400de0 <__printf_chk@plt>
  40133a:	48 8b 3d 3f 41 20 00 	mov    0x20413f(%rip),%rdi        # 605480 <buf_offset>
  401341:	e8 0d 0e 00 00       	callq  402153 <launch>
  401346:	b8 00 00 00 00       	mov    $0x0,%eax
  40134b:	5b                   	pop    %rbx
  40134c:	5d                   	pop    %rbp
  40134d:	41 5c                	pop    %r12
  40134f:	41 5d                	pop    %r13
  401351:	41 5e                	pop    %r14
  401353:	c3                   	retq   
  401354:	44 89 f2             	mov    %r14d,%edx
  401357:	48 8d 35 ea 1e 00 00 	lea    0x1eea(%rip),%rsi        # 403248 <_IO_stdin_used+0x188>
  40135e:	bf 01 00 00 00       	mov    $0x1,%edi
  401363:	b8 00 00 00 00       	mov    $0x0,%eax
  401368:	e8 73 fa ff ff       	callq  400de0 <__printf_chk@plt>
  40136d:	b8 00 00 00 00       	mov    $0x0,%eax
  401372:	e8 e9 08 00 00       	callq  401c60 <check_fail>
  401377:	eb a5                	jmp    40131e <main+0x190>

0000000000401379 <scramble>:
  401379:	48 83 ec 38          	sub    $0x38,%rsp
  40137d:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  401384:	00 00 
  401386:	48 89 44 24 28       	mov    %rax,0x28(%rsp)
  40138b:	31 c0                	xor    %eax,%eax
  40138d:	eb 10                	jmp    40139f <scramble+0x26>
  40138f:	69 d0 fc 6d 00 00    	imul   $0x6dfc,%eax,%edx
  401395:	01 fa                	add    %edi,%edx
  401397:	89 c1                	mov    %eax,%ecx
  401399:	89 14 8c             	mov    %edx,(%rsp,%rcx,4)
  40139c:	83 c0 01             	add    $0x1,%eax
  40139f:	83 f8 09             	cmp    $0x9,%eax
  4013a2:	76 eb                	jbe    40138f <scramble+0x16>
  4013a4:	8b 44 24 18          	mov    0x18(%rsp),%eax
  4013a8:	69 c0 4e e4 00 00    	imul   $0xe44e,%eax,%eax
  4013ae:	89 44 24 18          	mov    %eax,0x18(%rsp)
  4013b2:	8b 44 24 1c          	mov    0x1c(%rsp),%eax
  4013b6:	69 c0 fe 03 00 00    	imul   $0x3fe,%eax,%eax
  4013bc:	89 44 24 1c          	mov    %eax,0x1c(%rsp)
  4013c0:	8b 44 24 04          	mov    0x4(%rsp),%eax
  4013c4:	69 c0 17 26 00 00    	imul   $0x2617,%eax,%eax
  4013ca:	89 44 24 04          	mov    %eax,0x4(%rsp)
  4013ce:	8b 44 24 1c          	mov    0x1c(%rsp),%eax
  4013d2:	69 c0 c0 8e 00 00    	imul   $0x8ec0,%eax,%eax
  4013d8:	89 44 24 1c          	mov    %eax,0x1c(%rsp)
  4013dc:	8b 44 24 1c          	mov    0x1c(%rsp),%eax
  4013e0:	69 c0 86 e0 00 00    	imul   $0xe086,%eax,%eax
  4013e6:	89 44 24 1c          	mov    %eax,0x1c(%rsp)
  4013ea:	8b 44 24 14          	mov    0x14(%rsp),%eax
  4013ee:	69 c0 e9 1f 00 00    	imul   $0x1fe9,%eax,%eax
  4013f4:	89 44 24 14          	mov    %eax,0x14(%rsp)
  4013f8:	8b 44 24 0c          	mov    0xc(%rsp),%eax
  4013fc:	69 c0 3e 71 00 00    	imul   $0x713e,%eax,%eax
  401402:	89 44 24 0c          	mov    %eax,0xc(%rsp)
  401406:	8b 44 24 10          	mov    0x10(%rsp),%eax
  40140a:	69 c0 00 1a 00 00    	imul   $0x1a00,%eax,%eax
  401410:	89 44 24 10          	mov    %eax,0x10(%rsp)
  401414:	8b 44 24 18          	mov    0x18(%rsp),%eax
  401418:	69 c0 b4 0d 00 00    	imul   $0xdb4,%eax,%eax
  40141e:	89 44 24 18          	mov    %eax,0x18(%rsp)
  401422:	8b 44 24 14          	mov    0x14(%rsp),%eax
  401426:	69 c0 1a b6 00 00    	imul   $0xb61a,%eax,%eax
  40142c:	89 44 24 14          	mov    %eax,0x14(%rsp)
  401430:	8b 44 24 04          	mov    0x4(%rsp),%eax
  401434:	69 c0 81 51 00 00    	imul   $0x5181,%eax,%eax
  40143a:	89 44 24 04          	mov    %eax,0x4(%rsp)
  40143e:	8b 44 24 08          	mov    0x8(%rsp),%eax
  401442:	69 c0 1c d1 00 00    	imul   $0xd11c,%eax,%eax
  401448:	89 44 24 08          	mov    %eax,0x8(%rsp)
  40144c:	8b 04 24             	mov    (%rsp),%eax
  40144f:	69 c0 a2 8a 00 00    	imul   $0x8aa2,%eax,%eax
  401455:	89 04 24             	mov    %eax,(%rsp)
  401458:	8b 44 24 0c          	mov    0xc(%rsp),%eax
  40145c:	69 c0 ef 0d 00 00    	imul   $0xdef,%eax,%eax
  401462:	89 44 24 0c          	mov    %eax,0xc(%rsp)
  401466:	8b 44 24 18          	mov    0x18(%rsp),%eax
  40146a:	69 c0 72 63 00 00    	imul   $0x6372,%eax,%eax
  401470:	89 44 24 18          	mov    %eax,0x18(%rsp)
  401474:	8b 44 24 18          	mov    0x18(%rsp),%eax
  401478:	69 c0 3f 59 00 00    	imul   $0x593f,%eax,%eax
  40147e:	89 44 24 18          	mov    %eax,0x18(%rsp)
  401482:	8b 44 24 14          	mov    0x14(%rsp),%eax
  401486:	69 c0 b1 b2 00 00    	imul   $0xb2b1,%eax,%eax
  40148c:	89 44 24 14          	mov    %eax,0x14(%rsp)
  401490:	8b 44 24 08          	mov    0x8(%rsp),%eax
  401494:	69 c0 31 c7 00 00    	imul   $0xc731,%eax,%eax
  40149a:	89 44 24 08          	mov    %eax,0x8(%rsp)
  40149e:	8b 44 24 08          	mov    0x8(%rsp),%eax
  4014a2:	69 c0 ca c4 00 00    	imul   $0xc4ca,%eax,%eax
  4014a8:	89 44 24 08          	mov    %eax,0x8(%rsp)
  4014ac:	8b 44 24 14          	mov    0x14(%rsp),%eax
  4014b0:	69 c0 47 b4 00 00    	imul   $0xb447,%eax,%eax
  4014b6:	89 44 24 14          	mov    %eax,0x14(%rsp)
  4014ba:	8b 44 24 20          	mov    0x20(%rsp),%eax
  4014be:	69 c0 03 d6 00 00    	imul   $0xd603,%eax,%eax
  4014c4:	89 44 24 20          	mov    %eax,0x20(%rsp)
  4014c8:	8b 44 24 1c          	mov    0x1c(%rsp),%eax
  4014cc:	69 c0 94 25 00 00    	imul   $0x2594,%eax,%eax
  4014d2:	89 44 24 1c          	mov    %eax,0x1c(%rsp)
  4014d6:	8b 44 24 18          	mov    0x18(%rsp),%eax
  4014da:	69 c0 2d 87 00 00    	imul   $0x872d,%eax,%eax
  4014e0:	89 44 24 18          	mov    %eax,0x18(%rsp)
  4014e4:	8b 44 24 10          	mov    0x10(%rsp),%eax
  4014e8:	69 c0 e5 fe 00 00    	imul   $0xfee5,%eax,%eax
  4014ee:	89 44 24 10          	mov    %eax,0x10(%rsp)
  4014f2:	8b 44 24 10          	mov    0x10(%rsp),%eax
  4014f6:	69 c0 76 a5 00 00    	imul   $0xa576,%eax,%eax
  4014fc:	89 44 24 10          	mov    %eax,0x10(%rsp)
  401500:	8b 44 24 10          	mov    0x10(%rsp),%eax
  401504:	69 c0 41 55 00 00    	imul   $0x5541,%eax,%eax
  40150a:	89 44 24 10          	mov    %eax,0x10(%rsp)
  40150e:	8b 44 24 04          	mov    0x4(%rsp),%eax
  401512:	69 c0 91 5e 00 00    	imul   $0x5e91,%eax,%eax
  401518:	89 44 24 04          	mov    %eax,0x4(%rsp)
  40151c:	8b 44 24 20          	mov    0x20(%rsp),%eax
  401520:	69 c0 94 3f 00 00    	imul   $0x3f94,%eax,%eax
  401526:	89 44 24 20          	mov    %eax,0x20(%rsp)
  40152a:	8b 44 24 08          	mov    0x8(%rsp),%eax
  40152e:	69 c0 04 3d 00 00    	imul   $0x3d04,%eax,%eax
  401534:	89 44 24 08          	mov    %eax,0x8(%rsp)
  401538:	8b 44 24 18          	mov    0x18(%rsp),%eax
  40153c:	69 c0 77 d4 00 00    	imul   $0xd477,%eax,%eax
  401542:	89 44 24 18          	mov    %eax,0x18(%rsp)
  401546:	8b 44 24 08          	mov    0x8(%rsp),%eax
  40154a:	69 c0 fe bf 00 00    	imul   $0xbffe,%eax,%eax
  401550:	89 44 24 08          	mov    %eax,0x8(%rsp)
  401554:	8b 44 24 0c          	mov    0xc(%rsp),%eax
  401558:	69 c0 36 e8 00 00    	imul   $0xe836,%eax,%eax
  40155e:	89 44 24 0c          	mov    %eax,0xc(%rsp)
  401562:	8b 04 24             	mov    (%rsp),%eax
  401565:	69 c0 f3 8a 00 00    	imul   $0x8af3,%eax,%eax
  40156b:	89 04 24             	mov    %eax,(%rsp)
  40156e:	8b 44 24 1c          	mov    0x1c(%rsp),%eax
  401572:	69 c0 77 4b 00 00    	imul   $0x4b77,%eax,%eax
  401578:	89 44 24 1c          	mov    %eax,0x1c(%rsp)
  40157c:	8b 44 24 20          	mov    0x20(%rsp),%eax
  401580:	69 c0 5a 05 00 00    	imul   $0x55a,%eax,%eax
  401586:	89 44 24 20          	mov    %eax,0x20(%rsp)
  40158a:	8b 44 24 14          	mov    0x14(%rsp),%eax
  40158e:	69 c0 0e f5 00 00    	imul   $0xf50e,%eax,%eax
  401594:	89 44 24 14          	mov    %eax,0x14(%rsp)
  401598:	8b 44 24 20          	mov    0x20(%rsp),%eax
  40159c:	69 c0 ef d3 00 00    	imul   $0xd3ef,%eax,%eax
  4015a2:	89 44 24 20          	mov    %eax,0x20(%rsp)
  4015a6:	8b 44 24 08          	mov    0x8(%rsp),%eax
  4015aa:	69 c0 9e 22 00 00    	imul   $0x229e,%eax,%eax
  4015b0:	89 44 24 08          	mov    %eax,0x8(%rsp)
  4015b4:	8b 44 24 14          	mov    0x14(%rsp),%eax
  4015b8:	69 c0 37 5a 00 00    	imul   $0x5a37,%eax,%eax
  4015be:	89 44 24 14          	mov    %eax,0x14(%rsp)
  4015c2:	8b 44 24 24          	mov    0x24(%rsp),%eax
  4015c6:	69 c0 fe ef 00 00    	imul   $0xeffe,%eax,%eax
  4015cc:	89 44 24 24          	mov    %eax,0x24(%rsp)
  4015d0:	8b 44 24 1c          	mov    0x1c(%rsp),%eax
  4015d4:	69 c0 83 d6 00 00    	imul   $0xd683,%eax,%eax
  4015da:	89 44 24 1c          	mov    %eax,0x1c(%rsp)
  4015de:	8b 44 24 1c          	mov    0x1c(%rsp),%eax
  4015e2:	69 c0 a5 f6 00 00    	imul   $0xf6a5,%eax,%eax
  4015e8:	89 44 24 1c          	mov    %eax,0x1c(%rsp)
  4015ec:	8b 44 24 10          	mov    0x10(%rsp),%eax
  4015f0:	69 c0 9d 6b 00 00    	imul   $0x6b9d,%eax,%eax
  4015f6:	89 44 24 10          	mov    %eax,0x10(%rsp)
  4015fa:	8b 44 24 1c          	mov    0x1c(%rsp),%eax
  4015fe:	69 c0 49 1e 00 00    	imul   $0x1e49,%eax,%eax
  401604:	89 44 24 1c          	mov    %eax,0x1c(%rsp)
  401608:	8b 44 24 20          	mov    0x20(%rsp),%eax
  40160c:	69 c0 be 83 00 00    	imul   $0x83be,%eax,%eax
  401612:	89 44 24 20          	mov    %eax,0x20(%rsp)
  401616:	8b 44 24 20          	mov    0x20(%rsp),%eax
  40161a:	69 c0 17 49 00 00    	imul   $0x4917,%eax,%eax
  401620:	89 44 24 20          	mov    %eax,0x20(%rsp)
  401624:	8b 44 24 24          	mov    0x24(%rsp),%eax
  401628:	69 c0 76 20 00 00    	imul   $0x2076,%eax,%eax
  40162e:	89 44 24 24          	mov    %eax,0x24(%rsp)
  401632:	8b 44 24 04          	mov    0x4(%rsp),%eax
  401636:	69 c0 6c b6 00 00    	imul   $0xb66c,%eax,%eax
  40163c:	89 44 24 04          	mov    %eax,0x4(%rsp)
  401640:	8b 44 24 04          	mov    0x4(%rsp),%eax
  401644:	69 c0 b5 e0 00 00    	imul   $0xe0b5,%eax,%eax
  40164a:	89 44 24 04          	mov    %eax,0x4(%rsp)
  40164e:	8b 44 24 0c          	mov    0xc(%rsp),%eax
  401652:	69 c0 26 fb 00 00    	imul   $0xfb26,%eax,%eax
  401658:	89 44 24 0c          	mov    %eax,0xc(%rsp)
  40165c:	8b 44 24 14          	mov    0x14(%rsp),%eax
  401660:	69 c0 a2 01 00 00    	imul   $0x1a2,%eax,%eax
  401666:	89 44 24 14          	mov    %eax,0x14(%rsp)
  40166a:	8b 44 24 24          	mov    0x24(%rsp),%eax
  40166e:	69 c0 6c c1 00 00    	imul   $0xc16c,%eax,%eax
  401674:	89 44 24 24          	mov    %eax,0x24(%rsp)
  401678:	8b 44 24 08          	mov    0x8(%rsp),%eax
  40167c:	69 c0 d7 0a 00 00    	imul   $0xad7,%eax,%eax
  401682:	89 44 24 08          	mov    %eax,0x8(%rsp)
  401686:	8b 44 24 08          	mov    0x8(%rsp),%eax
  40168a:	69 c0 ad 30 00 00    	imul   $0x30ad,%eax,%eax
  401690:	89 44 24 08          	mov    %eax,0x8(%rsp)
  401694:	8b 44 24 08          	mov    0x8(%rsp),%eax
  401698:	69 c0 8e 05 00 00    	imul   $0x58e,%eax,%eax
  40169e:	89 44 24 08          	mov    %eax,0x8(%rsp)
  4016a2:	8b 44 24 24          	mov    0x24(%rsp),%eax
  4016a6:	69 c0 bc 64 00 00    	imul   $0x64bc,%eax,%eax
  4016ac:	89 44 24 24          	mov    %eax,0x24(%rsp)
  4016b0:	8b 44 24 24          	mov    0x24(%rsp),%eax
  4016b4:	69 c0 eb 65 00 00    	imul   $0x65eb,%eax,%eax
  4016ba:	89 44 24 24          	mov    %eax,0x24(%rsp)
  4016be:	8b 44 24 08          	mov    0x8(%rsp),%eax
  4016c2:	69 c0 26 11 00 00    	imul   $0x1126,%eax,%eax
  4016c8:	89 44 24 08          	mov    %eax,0x8(%rsp)
  4016cc:	8b 44 24 14          	mov    0x14(%rsp),%eax
  4016d0:	69 c0 9b 91 00 00    	imul   $0x919b,%eax,%eax
  4016d6:	89 44 24 14          	mov    %eax,0x14(%rsp)
  4016da:	8b 44 24 04          	mov    0x4(%rsp),%eax
  4016de:	69 c0 29 7a 00 00    	imul   $0x7a29,%eax,%eax
  4016e4:	89 44 24 04          	mov    %eax,0x4(%rsp)
  4016e8:	8b 44 24 10          	mov    0x10(%rsp),%eax
  4016ec:	69 c0 2f 22 00 00    	imul   $0x222f,%eax,%eax
  4016f2:	89 44 24 10          	mov    %eax,0x10(%rsp)
  4016f6:	8b 44 24 14          	mov    0x14(%rsp),%eax
  4016fa:	69 c0 16 59 00 00    	imul   $0x5916,%eax,%eax
  401700:	89 44 24 14          	mov    %eax,0x14(%rsp)
  401704:	8b 44 24 10          	mov    0x10(%rsp),%eax
  401708:	69 c0 8e f4 00 00    	imul   $0xf48e,%eax,%eax
  40170e:	89 44 24 10          	mov    %eax,0x10(%rsp)
  401712:	8b 44 24 14          	mov    0x14(%rsp),%eax
  401716:	69 c0 0e ee 00 00    	imul   $0xee0e,%eax,%eax
  40171c:	89 44 24 14          	mov    %eax,0x14(%rsp)
  401720:	8b 44 24 1c          	mov    0x1c(%rsp),%eax
  401724:	69 c0 65 11 00 00    	imul   $0x1165,%eax,%eax
  40172a:	89 44 24 1c          	mov    %eax,0x1c(%rsp)
  40172e:	8b 44 24 1c          	mov    0x1c(%rsp),%eax
  401732:	69 c0 53 bb 00 00    	imul   $0xbb53,%eax,%eax
  401738:	89 44 24 1c          	mov    %eax,0x1c(%rsp)
  40173c:	8b 44 24 24          	mov    0x24(%rsp),%eax
  401740:	69 c0 6a d9 00 00    	imul   $0xd96a,%eax,%eax
  401746:	89 44 24 24          	mov    %eax,0x24(%rsp)
  40174a:	8b 44 24 24          	mov    0x24(%rsp),%eax
  40174e:	69 c0 98 e8 00 00    	imul   $0xe898,%eax,%eax
  401754:	89 44 24 24          	mov    %eax,0x24(%rsp)
  401758:	8b 44 24 08          	mov    0x8(%rsp),%eax
  40175c:	69 c0 ca 60 00 00    	imul   $0x60ca,%eax,%eax
  401762:	89 44 24 08          	mov    %eax,0x8(%rsp)
  401766:	8b 44 24 10          	mov    0x10(%rsp),%eax
  40176a:	69 c0 50 49 00 00    	imul   $0x4950,%eax,%eax
  401770:	89 44 24 10          	mov    %eax,0x10(%rsp)
  401774:	8b 44 24 20          	mov    0x20(%rsp),%eax
  401778:	69 c0 f2 39 00 00    	imul   $0x39f2,%eax,%eax
  40177e:	89 44 24 20          	mov    %eax,0x20(%rsp)
  401782:	8b 44 24 14          	mov    0x14(%rsp),%eax
  401786:	69 c0 69 42 00 00    	imul   $0x4269,%eax,%eax
  40178c:	89 44 24 14          	mov    %eax,0x14(%rsp)
  401790:	8b 44 24 08          	mov    0x8(%rsp),%eax
  401794:	69 c0 6d 09 00 00    	imul   $0x96d,%eax,%eax
  40179a:	89 44 24 08          	mov    %eax,0x8(%rsp)
  40179e:	8b 44 24 04          	mov    0x4(%rsp),%eax
  4017a2:	69 c0 66 71 00 00    	imul   $0x7166,%eax,%eax
  4017a8:	89 44 24 04          	mov    %eax,0x4(%rsp)
  4017ac:	8b 44 24 24          	mov    0x24(%rsp),%eax
  4017b0:	69 c0 ff 82 00 00    	imul   $0x82ff,%eax,%eax
  4017b6:	89 44 24 24          	mov    %eax,0x24(%rsp)
  4017ba:	8b 44 24 14          	mov    0x14(%rsp),%eax
  4017be:	69 c0 62 b0 00 00    	imul   $0xb062,%eax,%eax
  4017c4:	89 44 24 14          	mov    %eax,0x14(%rsp)
  4017c8:	8b 44 24 10          	mov    0x10(%rsp),%eax
  4017cc:	69 c0 d0 ae 00 00    	imul   $0xaed0,%eax,%eax
  4017d2:	89 44 24 10          	mov    %eax,0x10(%rsp)
  4017d6:	8b 04 24             	mov    (%rsp),%eax
  4017d9:	69 c0 d4 e7 00 00    	imul   $0xe7d4,%eax,%eax
  4017df:	89 04 24             	mov    %eax,(%rsp)
  4017e2:	8b 44 24 24          	mov    0x24(%rsp),%eax
  4017e6:	69 c0 fa 18 00 00    	imul   $0x18fa,%eax,%eax
  4017ec:	89 44 24 24          	mov    %eax,0x24(%rsp)
  4017f0:	8b 44 24 18          	mov    0x18(%rsp),%eax
  4017f4:	69 c0 a1 78 00 00    	imul   $0x78a1,%eax,%eax
  4017fa:	89 44 24 18          	mov    %eax,0x18(%rsp)
  4017fe:	8b 44 24 20          	mov    0x20(%rsp),%eax
  401802:	69 c0 18 bf 00 00    	imul   $0xbf18,%eax,%eax
  401808:	89 44 24 20          	mov    %eax,0x20(%rsp)
  40180c:	8b 44 24 10          	mov    0x10(%rsp),%eax
  401810:	69 c0 67 3d 00 00    	imul   $0x3d67,%eax,%eax
  401816:	89 44 24 10          	mov    %eax,0x10(%rsp)
  40181a:	8b 44 24 08          	mov    0x8(%rsp),%eax
  40181e:	69 c0 b4 b0 00 00    	imul   $0xb0b4,%eax,%eax
  401824:	89 44 24 08          	mov    %eax,0x8(%rsp)
  401828:	8b 44 24 08          	mov    0x8(%rsp),%eax
  40182c:	69 c0 c3 7c 00 00    	imul   $0x7cc3,%eax,%eax
  401832:	89 44 24 08          	mov    %eax,0x8(%rsp)
  401836:	8b 44 24 24          	mov    0x24(%rsp),%eax
  40183a:	69 c0 46 3f 00 00    	imul   $0x3f46,%eax,%eax
  401840:	89 44 24 24          	mov    %eax,0x24(%rsp)
  401844:	8b 04 24             	mov    (%rsp),%eax
  401847:	69 c0 d6 77 00 00    	imul   $0x77d6,%eax,%eax
  40184d:	89 04 24             	mov    %eax,(%rsp)
  401850:	8b 44 24 04          	mov    0x4(%rsp),%eax
  401854:	69 c0 1f 74 00 00    	imul   $0x741f,%eax,%eax
  40185a:	89 44 24 04          	mov    %eax,0x4(%rsp)
  40185e:	8b 44 24 14          	mov    0x14(%rsp),%eax
  401862:	69 c0 15 19 00 00    	imul   $0x1915,%eax,%eax
  401868:	89 44 24 14          	mov    %eax,0x14(%rsp)
  40186c:	8b 44 24 14          	mov    0x14(%rsp),%eax
  401870:	69 c0 cf 0d 00 00    	imul   $0xdcf,%eax,%eax
  401876:	89 44 24 14          	mov    %eax,0x14(%rsp)
  40187a:	8b 44 24 14          	mov    0x14(%rsp),%eax
  40187e:	69 c0 96 18 00 00    	imul   $0x1896,%eax,%eax
  401884:	89 44 24 14          	mov    %eax,0x14(%rsp)
  401888:	8b 44 24 08          	mov    0x8(%rsp),%eax
  40188c:	69 c0 0b 76 00 00    	imul   $0x760b,%eax,%eax
  401892:	89 44 24 08          	mov    %eax,0x8(%rsp)
  401896:	8b 44 24 04          	mov    0x4(%rsp),%eax
  40189a:	69 c0 fe 35 00 00    	imul   $0x35fe,%eax,%eax
  4018a0:	89 44 24 04          	mov    %eax,0x4(%rsp)
  4018a4:	ba 00 00 00 00       	mov    $0x0,%edx
  4018a9:	b8 00 00 00 00       	mov    $0x0,%eax
  4018ae:	eb 0a                	jmp    4018ba <scramble+0x541>
  4018b0:	89 d1                	mov    %edx,%ecx
  4018b2:	8b 0c 8c             	mov    (%rsp,%rcx,4),%ecx
  4018b5:	01 c8                	add    %ecx,%eax
  4018b7:	83 c2 01             	add    $0x1,%edx
  4018ba:	83 fa 09             	cmp    $0x9,%edx
  4018bd:	76 f1                	jbe    4018b0 <scramble+0x537>
  4018bf:	48 8b 74 24 28       	mov    0x28(%rsp),%rsi
  4018c4:	64 48 33 34 25 28 00 	xor    %fs:0x28,%rsi
  4018cb:	00 00 
  4018cd:	75 05                	jne    4018d4 <scramble+0x55b>
  4018cf:	48 83 c4 38          	add    $0x38,%rsp
  4018d3:	c3                   	retq   
  4018d4:	e8 07 f4 ff ff       	callq  400ce0 <__stack_chk_fail@plt>

00000000004018d9 <getbuf>:
  4018d9:	48 83 ec 28          	sub    $0x28,%rsp
  4018dd:	48 89 e7             	mov    %rsp,%rdi
  4018e0:	e8 b3 03 00 00       	callq  401c98 <Gets>
  4018e5:	b8 01 00 00 00       	mov    $0x1,%eax
  4018ea:	48 83 c4 28          	add    $0x28,%rsp
  4018ee:	c3                   	retq   

00000000004018ef <touch1>:
  4018ef:	48 83 ec 08          	sub    $0x8,%rsp
  4018f3:	c7 05 ff 3b 20 00 01 	movl   $0x1,0x203bff(%rip)        # 6054fc <vlevel>
  4018fa:	00 00 00 
  4018fd:	48 8d 3d c3 1a 00 00 	lea    0x1ac3(%rip),%rdi        # 4033c7 <_IO_stdin_used+0x307>
  401904:	e8 b7 f3 ff ff       	callq  400cc0 <puts@plt>
  401909:	bf 01 00 00 00       	mov    $0x1,%edi
  40190e:	e8 f5 05 00 00       	callq  401f08 <validate>
  401913:	bf 00 00 00 00       	mov    $0x0,%edi
  401918:	e8 13 f5 ff ff       	callq  400e30 <exit@plt>

000000000040191d <touch2>:
  40191d:	48 83 ec 08          	sub    $0x8,%rsp
  401921:	89 fa                	mov    %edi,%edx
  401923:	c7 05 cf 3b 20 00 02 	movl   $0x2,0x203bcf(%rip)        # 6054fc <vlevel>
  40192a:	00 00 00 
  40192d:	39 3d d1 3b 20 00    	cmp    %edi,0x203bd1(%rip)        # 605504 <cookie>
  401933:	74 2a                	je     40195f <touch2+0x42>
  401935:	48 8d 35 dc 1a 00 00 	lea    0x1adc(%rip),%rsi        # 403418 <_IO_stdin_used+0x358>
  40193c:	bf 01 00 00 00       	mov    $0x1,%edi
  401941:	b8 00 00 00 00       	mov    $0x0,%eax
  401946:	e8 95 f4 ff ff       	callq  400de0 <__printf_chk@plt>
  40194b:	bf 02 00 00 00       	mov    $0x2,%edi
  401950:	e8 83 06 00 00       	callq  401fd8 <fail>
  401955:	bf 00 00 00 00       	mov    $0x0,%edi
  40195a:	e8 d1 f4 ff ff       	callq  400e30 <exit@plt>
  40195f:	48 8d 35 8a 1a 00 00 	lea    0x1a8a(%rip),%rsi        # 4033f0 <_IO_stdin_used+0x330>
  401966:	bf 01 00 00 00       	mov    $0x1,%edi
  40196b:	b8 00 00 00 00       	mov    $0x0,%eax
  401970:	e8 6b f4 ff ff       	callq  400de0 <__printf_chk@plt>
  401975:	bf 02 00 00 00       	mov    $0x2,%edi
  40197a:	e8 89 05 00 00       	callq  401f08 <validate>
  40197f:	eb d4                	jmp    401955 <touch2+0x38>

0000000000401981 <hexmatch>:
  401981:	41 54                	push   %r12
  401983:	55                   	push   %rbp
  401984:	53                   	push   %rbx
  401985:	48 83 c4 80          	add    $0xffffffffffffff80,%rsp
  401989:	89 fd                	mov    %edi,%ebp
  40198b:	48 89 f3             	mov    %rsi,%rbx
  40198e:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  401995:	00 00 
  401997:	48 89 44 24 78       	mov    %rax,0x78(%rsp)
  40199c:	31 c0                	xor    %eax,%eax
  40199e:	e8 fd f3 ff ff       	callq  400da0 <random@plt>
  4019a3:	48 89 c1             	mov    %rax,%rcx
  4019a6:	48 ba 0b d7 a3 70 3d 	movabs $0xa3d70a3d70a3d70b,%rdx
  4019ad:	0a d7 a3 
  4019b0:	48 f7 ea             	imul   %rdx
  4019b3:	48 01 ca             	add    %rcx,%rdx
  4019b6:	48 c1 fa 06          	sar    $0x6,%rdx
  4019ba:	48 89 c8             	mov    %rcx,%rax
  4019bd:	48 c1 f8 3f          	sar    $0x3f,%rax
  4019c1:	48 29 c2             	sub    %rax,%rdx
  4019c4:	48 8d 04 92          	lea    (%rdx,%rdx,4),%rax
  4019c8:	48 8d 14 80          	lea    (%rax,%rax,4),%rdx
  4019cc:	48 8d 04 95 00 00 00 	lea    0x0(,%rdx,4),%rax
  4019d3:	00 
  4019d4:	48 29 c1             	sub    %rax,%rcx
  4019d7:	4c 8d 24 0c          	lea    (%rsp,%rcx,1),%r12
  4019db:	41 89 e8             	mov    %ebp,%r8d
  4019de:	48 8d 0d ff 19 00 00 	lea    0x19ff(%rip),%rcx        # 4033e4 <_IO_stdin_used+0x324>
  4019e5:	48 c7 c2 ff ff ff ff 	mov    $0xffffffffffffffff,%rdx
  4019ec:	be 01 00 00 00       	mov    $0x1,%esi
  4019f1:	4c 89 e7             	mov    %r12,%rdi
  4019f4:	b8 00 00 00 00       	mov    $0x0,%eax
  4019f9:	e8 62 f4 ff ff       	callq  400e60 <__sprintf_chk@plt>
  4019fe:	ba 09 00 00 00       	mov    $0x9,%edx
  401a03:	4c 89 e6             	mov    %r12,%rsi
  401a06:	48 89 df             	mov    %rbx,%rdi
  401a09:	e8 92 f2 ff ff       	callq  400ca0 <strncmp@plt>
  401a0e:	85 c0                	test   %eax,%eax
  401a10:	0f 94 c0             	sete   %al
  401a13:	48 8b 5c 24 78       	mov    0x78(%rsp),%rbx
  401a18:	64 48 33 1c 25 28 00 	xor    %fs:0x28,%rbx
  401a1f:	00 00 
  401a21:	75 0c                	jne    401a2f <hexmatch+0xae>
  401a23:	0f b6 c0             	movzbl %al,%eax
  401a26:	48 83 ec 80          	sub    $0xffffffffffffff80,%rsp
  401a2a:	5b                   	pop    %rbx
  401a2b:	5d                   	pop    %rbp
  401a2c:	41 5c                	pop    %r12
  401a2e:	c3                   	retq   
  401a2f:	e8 ac f2 ff ff       	callq  400ce0 <__stack_chk_fail@plt>

0000000000401a34 <touch3>:
  401a34:	53                   	push   %rbx
  401a35:	48 89 fb             	mov    %rdi,%rbx
  401a38:	c7 05 ba 3a 20 00 03 	movl   $0x3,0x203aba(%rip)        # 6054fc <vlevel>
  401a3f:	00 00 00 
  401a42:	48 89 fe             	mov    %rdi,%rsi
  401a45:	8b 3d b9 3a 20 00    	mov    0x203ab9(%rip),%edi        # 605504 <cookie>
  401a4b:	e8 31 ff ff ff       	callq  401981 <hexmatch>
  401a50:	85 c0                	test   %eax,%eax
  401a52:	74 2d                	je     401a81 <touch3+0x4d>
  401a54:	48 89 da             	mov    %rbx,%rdx
  401a57:	48 8d 35 e2 19 00 00 	lea    0x19e2(%rip),%rsi        # 403440 <_IO_stdin_used+0x380>
  401a5e:	bf 01 00 00 00       	mov    $0x1,%edi
  401a63:	b8 00 00 00 00       	mov    $0x0,%eax
  401a68:	e8 73 f3 ff ff       	callq  400de0 <__printf_chk@plt>
  401a6d:	bf 03 00 00 00       	mov    $0x3,%edi
  401a72:	e8 91 04 00 00       	callq  401f08 <validate>
  401a77:	bf 00 00 00 00       	mov    $0x0,%edi
  401a7c:	e8 af f3 ff ff       	callq  400e30 <exit@plt>
  401a81:	48 89 da             	mov    %rbx,%rdx
  401a84:	48 8d 35 dd 19 00 00 	lea    0x19dd(%rip),%rsi        # 403468 <_IO_stdin_used+0x3a8>
  401a8b:	bf 01 00 00 00       	mov    $0x1,%edi
  401a90:	b8 00 00 00 00       	mov    $0x0,%eax
  401a95:	e8 46 f3 ff ff       	callq  400de0 <__printf_chk@plt>
  401a9a:	bf 03 00 00 00       	mov    $0x3,%edi
  401a9f:	e8 34 05 00 00       	callq  401fd8 <fail>
  401aa4:	eb d1                	jmp    401a77 <touch3+0x43>

0000000000401aa6 <test>:
  401aa6:	48 83 ec 08          	sub    $0x8,%rsp
  401aaa:	b8 00 00 00 00       	mov    $0x0,%eax
  401aaf:	e8 25 fe ff ff       	callq  4018d9 <getbuf>
  401ab4:	89 c2                	mov    %eax,%edx
  401ab6:	48 8d 35 d3 19 00 00 	lea    0x19d3(%rip),%rsi        # 403490 <_IO_stdin_used+0x3d0>
  401abd:	bf 01 00 00 00       	mov    $0x1,%edi
  401ac2:	b8 00 00 00 00       	mov    $0x0,%eax
  401ac7:	e8 14 f3 ff ff       	callq  400de0 <__printf_chk@plt>
  401acc:	48 83 c4 08          	add    $0x8,%rsp
  401ad0:	c3                   	retq   

0000000000401ad1 <start_farm>:
  401ad1:	b8 01 00 00 00       	mov    $0x1,%eax
  401ad6:	c3                   	retq   

0000000000401ad7 <addval_286>:
  401ad7:	8d 87 6f 48 89 c7    	lea    -0x3876b791(%rdi),%eax
  401add:	c3                   	retq   

0000000000401ade <setval_173>:
  401ade:	c7 07 05 48 89 c7    	movl   $0xc7894805,(%rdi)
  401ae4:	c3                   	retq   

0000000000401ae5 <getval_158>:
  401ae5:	b8 08 4a 89 c7       	mov    $0xc7894a08,%eax
  401aea:	c3                   	retq   

0000000000401aeb <getval_296>:
  401aeb:	b8 58 90 90 90       	mov    $0x90909058,%eax
  401af0:	c3                   	retq   

0000000000401af1 <addval_293>:
  401af1:	8d 87 58 92 90 90    	lea    -0x6f6f6da8(%rdi),%eax
  401af7:	c3                   	retq   

0000000000401af8 <setval_470>:
  401af8:	c7 07 48 89 c7 94    	movl   $0x94c78948,(%rdi)
  401afe:	c3                   	retq   

0000000000401aff <getval_209>:
  401aff:	b8 58 90 90 c3       	mov    $0xc3909058,%eax
  401b04:	c3                   	retq   

0000000000401b05 <getval_281>:
  401b05:	b8 89 18 90 90       	mov    $0x90901889,%eax
  401b0a:	c3                   	retq   

0000000000401b0b <mid_farm>:
  401b0b:	b8 01 00 00 00       	mov    $0x1,%eax
  401b10:	c3                   	retq   

0000000000401b11 <add_xy>:
  401b11:	48 8d 04 37          	lea    (%rdi,%rsi,1),%rax
  401b15:	c3                   	retq   

0000000000401b16 <setval_137>:
  401b16:	c7 07 43 a9 d1 90    	movl   $0x90d1a943,(%rdi)
  401b1c:	c3                   	retq   

0000000000401b1d <addval_444>:
  401b1d:	8d 87 81 89 c2 91    	lea    -0x6e3d767f(%rdi),%eax
  401b23:	c3                   	retq   

0000000000401b24 <addval_180>:
  401b24:	8d 87 89 d1 90 c3    	lea    -0x3c6f2e77(%rdi),%eax
  401b2a:	c3                   	retq   

0000000000401b2b <getval_265>:
  401b2b:	b8 d5 89 d1 c2       	mov    $0xc2d189d5,%eax
  401b30:	c3                   	retq   

0000000000401b31 <addval_319>:
  401b31:	8d 87 99 d1 90 90    	lea    -0x6f6f2e67(%rdi),%eax
  401b37:	c3                   	retq   

0000000000401b38 <setval_424>:
  401b38:	c7 07 89 ce 78 c0    	movl   $0xc078ce89,(%rdi)
  401b3e:	c3                   	retq   

0000000000401b3f <setval_357>:
  401b3f:	c7 07 48 89 e0 92    	movl   $0x92e08948,(%rdi)
  401b45:	c3                   	retq   

0000000000401b46 <getval_123>:
  401b46:	b8 6d 03 89 c2       	mov    $0xc289036d,%eax
  401b4b:	c3                   	retq   

0000000000401b4c <getval_436>:
  401b4c:	b8 48 89 e0 91       	mov    $0x91e08948,%eax
  401b51:	c3                   	retq   

0000000000401b52 <addval_349>:
  401b52:	8d 87 09 4c 89 e0    	lea    -0x1f76b3f7(%rdi),%eax
  401b58:	c3                   	retq   

0000000000401b59 <setval_162>:
  401b59:	c7 07 89 c2 91 c3    	movl   $0xc391c289,(%rdi)
  401b5f:	c3                   	retq   

0000000000401b60 <addval_263>:
  401b60:	8d 87 8d ce 84 c0    	lea    -0x3f7b3173(%rdi),%eax
  401b66:	c3                   	retq   

0000000000401b67 <getval_482>:
  401b67:	b8 89 c2 28 d2       	mov    $0xd228c289,%eax
  401b6c:	c3                   	retq   

0000000000401b6d <setval_135>:
  401b6d:	c7 07 89 c2 78 db    	movl   $0xdb78c289,(%rdi)
  401b73:	c3                   	retq   

0000000000401b74 <setval_489>:
  401b74:	c7 07 4b 48 89 e0    	movl   $0xe089484b,(%rdi)
  401b7a:	c3                   	retq   

0000000000401b7b <addval_459>:
  401b7b:	8d 87 d8 89 ce 90    	lea    -0x6f317628(%rdi),%eax
  401b81:	c3                   	retq   

0000000000401b82 <setval_152>:
  401b82:	c7 07 89 d1 18 db    	movl   $0xdb18d189,(%rdi)
  401b88:	c3                   	retq   

0000000000401b89 <getval_327>:
  401b89:	b8 a5 8b c2 90       	mov    $0x90c28ba5,%eax
  401b8e:	c3                   	retq   

0000000000401b8f <getval_139>:
  401b8f:	b8 bc 89 ce c7       	mov    $0xc7ce89bc,%eax
  401b94:	c3                   	retq   

0000000000401b95 <setval_360>:
  401b95:	c7 07 48 89 e0 90    	movl   $0x90e08948,(%rdi)
  401b9b:	c3                   	retq   

0000000000401b9c <addval_233>:
  401b9c:	8d 87 89 d1 78 c0    	lea    -0x3f872e77(%rdi),%eax
  401ba2:	c3                   	retq   

0000000000401ba3 <addval_487>:
  401ba3:	8d 87 89 d1 18 db    	lea    -0x24e72e77(%rdi),%eax
  401ba9:	c3                   	retq   

0000000000401baa <setval_309>:
  401baa:	c7 07 89 ce c3 62    	movl   $0x62c3ce89,(%rdi)
  401bb0:	c3                   	retq   

0000000000401bb1 <getval_441>:
  401bb1:	b8 08 89 e0 c3       	mov    $0xc3e08908,%eax
  401bb6:	c3                   	retq   

0000000000401bb7 <getval_237>:
  401bb7:	b8 89 d1 90 c3       	mov    $0xc390d189,%eax
  401bbc:	c3                   	retq   

0000000000401bbd <setval_396>:
  401bbd:	c7 07 48 89 e0 94    	movl   $0x94e08948,(%rdi)
  401bc3:	c3                   	retq   

0000000000401bc4 <getval_367>:
  401bc4:	b8 89 ce 28 c0       	mov    $0xc028ce89,%eax
  401bc9:	c3                   	retq   

0000000000401bca <getval_474>:
  401bca:	b8 81 ce 90 90       	mov    $0x9090ce81,%eax
  401bcf:	c3                   	retq   

0000000000401bd0 <getval_300>:
  401bd0:	b8 7e 89 c2 94       	mov    $0x94c2897e,%eax
  401bd5:	c3                   	retq   

0000000000401bd6 <setval_462>:
  401bd6:	c7 07 90 55 89 c2    	movl   $0xc2895590,(%rdi)
  401bdc:	c3                   	retq   

0000000000401bdd <addval_467>:
  401bdd:	8d 87 89 ce 92 90    	lea    -0x6f6d3177(%rdi),%eax
  401be3:	c3                   	retq   

0000000000401be4 <getval_341>:
  401be4:	b8 48 89 e0 c2       	mov    $0xc2e08948,%eax
  401be9:	c3                   	retq   

0000000000401bea <end_farm>:
  401bea:	b8 01 00 00 00       	mov    $0x1,%eax
  401bef:	c3                   	retq   

0000000000401bf0 <save_char>:
  401bf0:	8b 05 2e 45 20 00    	mov    0x20452e(%rip),%eax        # 606124 <gets_cnt>
  401bf6:	3d ff 03 00 00       	cmp    $0x3ff,%eax
  401bfb:	7f 4a                	jg     401c47 <save_char+0x57>
  401bfd:	89 f9                	mov    %edi,%ecx
  401bff:	c0 e9 04             	shr    $0x4,%cl
  401c02:	8d 14 40             	lea    (%rax,%rax,2),%edx
  401c05:	4c 8d 05 a4 1b 00 00 	lea    0x1ba4(%rip),%r8        # 4037b0 <trans_char>
  401c0c:	83 e1 0f             	and    $0xf,%ecx
  401c0f:	45 0f b6 0c 08       	movzbl (%r8,%rcx,1),%r9d
  401c14:	48 8d 0d 05 39 20 00 	lea    0x203905(%rip),%rcx        # 605520 <gets_buf>
  401c1b:	48 63 f2             	movslq %edx,%rsi
  401c1e:	44 88 0c 31          	mov    %r9b,(%rcx,%rsi,1)
  401c22:	8d 72 01             	lea    0x1(%rdx),%esi
  401c25:	83 e7 0f             	and    $0xf,%edi
  401c28:	41 0f b6 3c 38       	movzbl (%r8,%rdi,1),%edi
  401c2d:	48 63 f6             	movslq %esi,%rsi
  401c30:	40 88 3c 31          	mov    %dil,(%rcx,%rsi,1)
  401c34:	83 c2 02             	add    $0x2,%edx
  401c37:	48 63 d2             	movslq %edx,%rdx
  401c3a:	c6 04 11 20          	movb   $0x20,(%rcx,%rdx,1)
  401c3e:	83 c0 01             	add    $0x1,%eax
  401c41:	89 05 dd 44 20 00    	mov    %eax,0x2044dd(%rip)        # 606124 <gets_cnt>
  401c47:	f3 c3                	repz retq 

0000000000401c49 <save_term>:
  401c49:	8b 05 d5 44 20 00    	mov    0x2044d5(%rip),%eax        # 606124 <gets_cnt>
  401c4f:	8d 04 40             	lea    (%rax,%rax,2),%eax
  401c52:	48 98                	cltq   
  401c54:	48 8d 15 c5 38 20 00 	lea    0x2038c5(%rip),%rdx        # 605520 <gets_buf>
  401c5b:	c6 04 02 00          	movb   $0x0,(%rdx,%rax,1)
  401c5f:	c3                   	retq   

0000000000401c60 <check_fail>:
  401c60:	48 83 ec 08          	sub    $0x8,%rsp
  401c64:	0f be 15 bd 44 20 00 	movsbl 0x2044bd(%rip),%edx        # 606128 <target_prefix>
  401c6b:	4c 8d 05 ae 38 20 00 	lea    0x2038ae(%rip),%r8        # 605520 <gets_buf>
  401c72:	8b 0d 80 38 20 00    	mov    0x203880(%rip),%ecx        # 6054f8 <check_level>
  401c78:	48 8d 35 34 18 00 00 	lea    0x1834(%rip),%rsi        # 4034b3 <_IO_stdin_used+0x3f3>
  401c7f:	bf 01 00 00 00       	mov    $0x1,%edi
  401c84:	b8 00 00 00 00       	mov    $0x0,%eax
  401c89:	e8 52 f1 ff ff       	callq  400de0 <__printf_chk@plt>
  401c8e:	bf 01 00 00 00       	mov    $0x1,%edi
  401c93:	e8 98 f1 ff ff       	callq  400e30 <exit@plt>

0000000000401c98 <Gets>:
  401c98:	41 54                	push   %r12
  401c9a:	55                   	push   %rbp
  401c9b:	53                   	push   %rbx
  401c9c:	49 89 fc             	mov    %rdi,%r12
  401c9f:	c7 05 7b 44 20 00 00 	movl   $0x0,0x20447b(%rip)        # 606124 <gets_cnt>
  401ca6:	00 00 00 
  401ca9:	48 89 fb             	mov    %rdi,%rbx
  401cac:	eb 11                	jmp    401cbf <Gets+0x27>
  401cae:	48 8d 6b 01          	lea    0x1(%rbx),%rbp
  401cb2:	88 03                	mov    %al,(%rbx)
  401cb4:	0f b6 f8             	movzbl %al,%edi
  401cb7:	e8 34 ff ff ff       	callq  401bf0 <save_char>
  401cbc:	48 89 eb             	mov    %rbp,%rbx
  401cbf:	48 8b 3d 2a 38 20 00 	mov    0x20382a(%rip),%rdi        # 6054f0 <infile>
  401cc6:	e8 e5 f0 ff ff       	callq  400db0 <_IO_getc@plt>
  401ccb:	83 f8 ff             	cmp    $0xffffffff,%eax
  401cce:	74 05                	je     401cd5 <Gets+0x3d>
  401cd0:	83 f8 0a             	cmp    $0xa,%eax
  401cd3:	75 d9                	jne    401cae <Gets+0x16>
  401cd5:	c6 03 00             	movb   $0x0,(%rbx)
  401cd8:	b8 00 00 00 00       	mov    $0x0,%eax
  401cdd:	e8 67 ff ff ff       	callq  401c49 <save_term>
  401ce2:	4c 89 e0             	mov    %r12,%rax
  401ce5:	5b                   	pop    %rbx
  401ce6:	5d                   	pop    %rbp
  401ce7:	41 5c                	pop    %r12
  401ce9:	c3                   	retq   

0000000000401cea <notify_server>:
  401cea:	55                   	push   %rbp
  401ceb:	53                   	push   %rbx
  401cec:	48 81 ec 18 40 00 00 	sub    $0x4018,%rsp
  401cf3:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  401cfa:	00 00 
  401cfc:	48 89 84 24 08 40 00 	mov    %rax,0x4008(%rsp)
  401d03:	00 
  401d04:	31 c0                	xor    %eax,%eax
  401d06:	83 3d fb 37 20 00 00 	cmpl   $0x0,0x2037fb(%rip)        # 605508 <is_checker>
  401d0d:	0f 85 d2 00 00 00    	jne    401de5 <notify_server+0xfb>
  401d13:	89 fb                	mov    %edi,%ebx
  401d15:	8b 05 09 44 20 00    	mov    0x204409(%rip),%eax        # 606124 <gets_cnt>
  401d1b:	83 c0 64             	add    $0x64,%eax
  401d1e:	3d 00 20 00 00       	cmp    $0x2000,%eax
  401d23:	0f 8f dd 00 00 00    	jg     401e06 <notify_server+0x11c>
  401d29:	0f be 05 f8 43 20 00 	movsbl 0x2043f8(%rip),%eax        # 606128 <target_prefix>
  401d30:	83 3d 51 37 20 00 00 	cmpl   $0x0,0x203751(%rip)        # 605488 <notify>
  401d37:	0f 84 e9 00 00 00    	je     401e26 <notify_server+0x13c>
  401d3d:	8b 15 bd 37 20 00    	mov    0x2037bd(%rip),%edx        # 605500 <authkey>
  401d43:	85 db                	test   %ebx,%ebx
  401d45:	0f 84 e5 00 00 00    	je     401e30 <notify_server+0x146>
  401d4b:	48 8d 2d 77 17 00 00 	lea    0x1777(%rip),%rbp        # 4034c9 <_IO_stdin_used+0x409>
  401d52:	48 89 e7             	mov    %rsp,%rdi
  401d55:	48 8d 0d c4 37 20 00 	lea    0x2037c4(%rip),%rcx        # 605520 <gets_buf>
  401d5c:	51                   	push   %rcx
  401d5d:	56                   	push   %rsi
  401d5e:	50                   	push   %rax
  401d5f:	52                   	push   %rdx
  401d60:	49 89 e9             	mov    %rbp,%r9
  401d63:	44 8b 05 c6 33 20 00 	mov    0x2033c6(%rip),%r8d        # 605130 <target_id>
  401d6a:	48 8d 0d 62 17 00 00 	lea    0x1762(%rip),%rcx        # 4034d3 <_IO_stdin_used+0x413>
  401d71:	ba 00 20 00 00       	mov    $0x2000,%edx
  401d76:	be 01 00 00 00       	mov    $0x1,%esi
  401d7b:	b8 00 00 00 00       	mov    $0x0,%eax
  401d80:	e8 db f0 ff ff       	callq  400e60 <__sprintf_chk@plt>
  401d85:	48 83 c4 20          	add    $0x20,%rsp
  401d89:	83 3d f8 36 20 00 00 	cmpl   $0x0,0x2036f8(%rip)        # 605488 <notify>
  401d90:	0f 84 df 00 00 00    	je     401e75 <notify_server+0x18b>
  401d96:	85 db                	test   %ebx,%ebx
  401d98:	0f 84 c6 00 00 00    	je     401e64 <notify_server+0x17a>
  401d9e:	48 89 e1             	mov    %rsp,%rcx
  401da1:	4c 8d 8c 24 00 20 00 	lea    0x2000(%rsp),%r9
  401da8:	00 
  401da9:	41 b8 00 00 00 00    	mov    $0x0,%r8d
  401daf:	48 8b 15 92 33 20 00 	mov    0x203392(%rip),%rdx        # 605148 <lab>
  401db6:	48 8b 35 93 33 20 00 	mov    0x203393(%rip),%rsi        # 605150 <course>
  401dbd:	48 8b 3d 7c 33 20 00 	mov    0x20337c(%rip),%rdi        # 605140 <user_id>
  401dc4:	e8 af 11 00 00       	callq  402f78 <driver_post>
  401dc9:	85 c0                	test   %eax,%eax
  401dcb:	78 6f                	js     401e3c <notify_server+0x152>
  401dcd:	48 8d 3d 44 18 00 00 	lea    0x1844(%rip),%rdi        # 403618 <_IO_stdin_used+0x558>
  401dd4:	e8 e7 ee ff ff       	callq  400cc0 <puts@plt>
  401dd9:	48 8d 3d 1b 17 00 00 	lea    0x171b(%rip),%rdi        # 4034fb <_IO_stdin_used+0x43b>
  401de0:	e8 db ee ff ff       	callq  400cc0 <puts@plt>
  401de5:	48 8b 84 24 08 40 00 	mov    0x4008(%rsp),%rax
  401dec:	00 
  401ded:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
  401df4:	00 00 
  401df6:	0f 85 07 01 00 00    	jne    401f03 <notify_server+0x219>
  401dfc:	48 81 c4 18 40 00 00 	add    $0x4018,%rsp
  401e03:	5b                   	pop    %rbx
  401e04:	5d                   	pop    %rbp
  401e05:	c3                   	retq   
  401e06:	48 8d 35 db 17 00 00 	lea    0x17db(%rip),%rsi        # 4035e8 <_IO_stdin_used+0x528>
  401e0d:	bf 01 00 00 00       	mov    $0x1,%edi
  401e12:	b8 00 00 00 00       	mov    $0x0,%eax
  401e17:	e8 c4 ef ff ff       	callq  400de0 <__printf_chk@plt>
  401e1c:	bf 01 00 00 00       	mov    $0x1,%edi
  401e21:	e8 0a f0 ff ff       	callq  400e30 <exit@plt>
  401e26:	ba ff ff ff ff       	mov    $0xffffffff,%edx
  401e2b:	e9 13 ff ff ff       	jmpq   401d43 <notify_server+0x59>
  401e30:	48 8d 2d 97 16 00 00 	lea    0x1697(%rip),%rbp        # 4034ce <_IO_stdin_used+0x40e>
  401e37:	e9 16 ff ff ff       	jmpq   401d52 <notify_server+0x68>
  401e3c:	48 8d 94 24 00 20 00 	lea    0x2000(%rsp),%rdx
  401e43:	00 
  401e44:	48 8d 35 a4 16 00 00 	lea    0x16a4(%rip),%rsi        # 4034ef <_IO_stdin_used+0x42f>
  401e4b:	bf 01 00 00 00       	mov    $0x1,%edi
  401e50:	b8 00 00 00 00       	mov    $0x0,%eax
  401e55:	e8 86 ef ff ff       	callq  400de0 <__printf_chk@plt>
  401e5a:	bf 01 00 00 00       	mov    $0x1,%edi
  401e5f:	e8 cc ef ff ff       	callq  400e30 <exit@plt>
  401e64:	48 8d 3d 9a 16 00 00 	lea    0x169a(%rip),%rdi        # 403505 <_IO_stdin_used+0x445>
  401e6b:	e8 50 ee ff ff       	callq  400cc0 <puts@plt>
  401e70:	e9 70 ff ff ff       	jmpq   401de5 <notify_server+0xfb>
  401e75:	48 89 ea             	mov    %rbp,%rdx
  401e78:	48 8d 35 d1 17 00 00 	lea    0x17d1(%rip),%rsi        # 403650 <_IO_stdin_used+0x590>
  401e7f:	bf 01 00 00 00       	mov    $0x1,%edi
  401e84:	b8 00 00 00 00       	mov    $0x0,%eax
  401e89:	e8 52 ef ff ff       	callq  400de0 <__printf_chk@plt>
  401e8e:	48 8b 15 ab 32 20 00 	mov    0x2032ab(%rip),%rdx        # 605140 <user_id>
  401e95:	48 8d 35 70 16 00 00 	lea    0x1670(%rip),%rsi        # 40350c <_IO_stdin_used+0x44c>
  401e9c:	bf 01 00 00 00       	mov    $0x1,%edi
  401ea1:	b8 00 00 00 00       	mov    $0x0,%eax
  401ea6:	e8 35 ef ff ff       	callq  400de0 <__printf_chk@plt>
  401eab:	48 8b 15 9e 32 20 00 	mov    0x20329e(%rip),%rdx        # 605150 <course>
  401eb2:	48 8d 35 60 16 00 00 	lea    0x1660(%rip),%rsi        # 403519 <_IO_stdin_used+0x459>
  401eb9:	bf 01 00 00 00       	mov    $0x1,%edi
  401ebe:	b8 00 00 00 00       	mov    $0x0,%eax
  401ec3:	e8 18 ef ff ff       	callq  400de0 <__printf_chk@plt>
  401ec8:	48 8b 15 79 32 20 00 	mov    0x203279(%rip),%rdx        # 605148 <lab>
  401ecf:	48 8d 35 4f 16 00 00 	lea    0x164f(%rip),%rsi        # 403525 <_IO_stdin_used+0x465>
  401ed6:	bf 01 00 00 00       	mov    $0x1,%edi
  401edb:	b8 00 00 00 00       	mov    $0x0,%eax
  401ee0:	e8 fb ee ff ff       	callq  400de0 <__printf_chk@plt>
  401ee5:	48 89 e2             	mov    %rsp,%rdx
  401ee8:	48 8d 35 3f 16 00 00 	lea    0x163f(%rip),%rsi        # 40352e <_IO_stdin_used+0x46e>
  401eef:	bf 01 00 00 00       	mov    $0x1,%edi
  401ef4:	b8 00 00 00 00       	mov    $0x0,%eax
  401ef9:	e8 e2 ee ff ff       	callq  400de0 <__printf_chk@plt>
  401efe:	e9 e2 fe ff ff       	jmpq   401de5 <notify_server+0xfb>
  401f03:	e8 d8 ed ff ff       	callq  400ce0 <__stack_chk_fail@plt>

0000000000401f08 <validate>:
  401f08:	53                   	push   %rbx
  401f09:	89 fb                	mov    %edi,%ebx
  401f0b:	83 3d f6 35 20 00 00 	cmpl   $0x0,0x2035f6(%rip)        # 605508 <is_checker>
  401f12:	74 72                	je     401f86 <validate+0x7e>
  401f14:	39 3d e2 35 20 00    	cmp    %edi,0x2035e2(%rip)        # 6054fc <vlevel>
  401f1a:	75 32                	jne    401f4e <validate+0x46>
  401f1c:	8b 15 d6 35 20 00    	mov    0x2035d6(%rip),%edx        # 6054f8 <check_level>
  401f22:	39 fa                	cmp    %edi,%edx
  401f24:	75 3e                	jne    401f64 <validate+0x5c>
  401f26:	0f be 15 fb 41 20 00 	movsbl 0x2041fb(%rip),%edx        # 606128 <target_prefix>
  401f2d:	4c 8d 05 ec 35 20 00 	lea    0x2035ec(%rip),%r8        # 605520 <gets_buf>
  401f34:	89 f9                	mov    %edi,%ecx
  401f36:	48 8d 35 1b 16 00 00 	lea    0x161b(%rip),%rsi        # 403558 <_IO_stdin_used+0x498>
  401f3d:	bf 01 00 00 00       	mov    $0x1,%edi
  401f42:	b8 00 00 00 00       	mov    $0x0,%eax
  401f47:	e8 94 ee ff ff       	callq  400de0 <__printf_chk@plt>
  401f4c:	5b                   	pop    %rbx
  401f4d:	c3                   	retq   
  401f4e:	48 8d 3d e5 15 00 00 	lea    0x15e5(%rip),%rdi        # 40353a <_IO_stdin_used+0x47a>
  401f55:	e8 66 ed ff ff       	callq  400cc0 <puts@plt>
  401f5a:	b8 00 00 00 00       	mov    $0x0,%eax
  401f5f:	e8 fc fc ff ff       	callq  401c60 <check_fail>
  401f64:	89 f9                	mov    %edi,%ecx
  401f66:	48 8d 35 0b 17 00 00 	lea    0x170b(%rip),%rsi        # 403678 <_IO_stdin_used+0x5b8>
  401f6d:	bf 01 00 00 00       	mov    $0x1,%edi
  401f72:	b8 00 00 00 00       	mov    $0x0,%eax
  401f77:	e8 64 ee ff ff       	callq  400de0 <__printf_chk@plt>
  401f7c:	b8 00 00 00 00       	mov    $0x0,%eax
  401f81:	e8 da fc ff ff       	callq  401c60 <check_fail>
  401f86:	39 3d 70 35 20 00    	cmp    %edi,0x203570(%rip)        # 6054fc <vlevel>
  401f8c:	74 1a                	je     401fa8 <validate+0xa0>
  401f8e:	48 8d 3d a5 15 00 00 	lea    0x15a5(%rip),%rdi        # 40353a <_IO_stdin_used+0x47a>
  401f95:	e8 26 ed ff ff       	callq  400cc0 <puts@plt>
  401f9a:	89 de                	mov    %ebx,%esi
  401f9c:	bf 00 00 00 00       	mov    $0x0,%edi
  401fa1:	e8 44 fd ff ff       	callq  401cea <notify_server>
  401fa6:	eb a4                	jmp    401f4c <validate+0x44>
  401fa8:	0f be 0d 79 41 20 00 	movsbl 0x204179(%rip),%ecx        # 606128 <target_prefix>
  401faf:	89 fa                	mov    %edi,%edx
  401fb1:	48 8d 35 e8 16 00 00 	lea    0x16e8(%rip),%rsi        # 4036a0 <_IO_stdin_used+0x5e0>
  401fb8:	bf 01 00 00 00       	mov    $0x1,%edi
  401fbd:	b8 00 00 00 00       	mov    $0x0,%eax
  401fc2:	e8 19 ee ff ff       	callq  400de0 <__printf_chk@plt>
  401fc7:	89 de                	mov    %ebx,%esi
  401fc9:	bf 01 00 00 00       	mov    $0x1,%edi
  401fce:	e8 17 fd ff ff       	callq  401cea <notify_server>
  401fd3:	e9 74 ff ff ff       	jmpq   401f4c <validate+0x44>

0000000000401fd8 <fail>:
  401fd8:	48 83 ec 08          	sub    $0x8,%rsp
  401fdc:	83 3d 25 35 20 00 00 	cmpl   $0x0,0x203525(%rip)        # 605508 <is_checker>
  401fe3:	75 11                	jne    401ff6 <fail+0x1e>
  401fe5:	89 fe                	mov    %edi,%esi
  401fe7:	bf 00 00 00 00       	mov    $0x0,%edi
  401fec:	e8 f9 fc ff ff       	callq  401cea <notify_server>
  401ff1:	48 83 c4 08          	add    $0x8,%rsp
  401ff5:	c3                   	retq   
  401ff6:	b8 00 00 00 00       	mov    $0x0,%eax
  401ffb:	e8 60 fc ff ff       	callq  401c60 <check_fail>

0000000000402000 <bushandler>:
  402000:	48 83 ec 08          	sub    $0x8,%rsp
  402004:	83 3d fd 34 20 00 00 	cmpl   $0x0,0x2034fd(%rip)        # 605508 <is_checker>
  40200b:	74 16                	je     402023 <bushandler+0x23>
  40200d:	48 8d 3d 59 15 00 00 	lea    0x1559(%rip),%rdi        # 40356d <_IO_stdin_used+0x4ad>
  402014:	e8 a7 ec ff ff       	callq  400cc0 <puts@plt>
  402019:	b8 00 00 00 00       	mov    $0x0,%eax
  40201e:	e8 3d fc ff ff       	callq  401c60 <check_fail>
  402023:	48 8d 3d ae 16 00 00 	lea    0x16ae(%rip),%rdi        # 4036d8 <_IO_stdin_used+0x618>
  40202a:	e8 91 ec ff ff       	callq  400cc0 <puts@plt>
  40202f:	48 8d 3d 41 15 00 00 	lea    0x1541(%rip),%rdi        # 403577 <_IO_stdin_used+0x4b7>
  402036:	e8 85 ec ff ff       	callq  400cc0 <puts@plt>
  40203b:	be 00 00 00 00       	mov    $0x0,%esi
  402040:	bf 00 00 00 00       	mov    $0x0,%edi
  402045:	e8 a0 fc ff ff       	callq  401cea <notify_server>
  40204a:	bf 01 00 00 00       	mov    $0x1,%edi
  40204f:	e8 dc ed ff ff       	callq  400e30 <exit@plt>

0000000000402054 <seghandler>:
  402054:	48 83 ec 08          	sub    $0x8,%rsp
  402058:	83 3d a9 34 20 00 00 	cmpl   $0x0,0x2034a9(%rip)        # 605508 <is_checker>
  40205f:	74 16                	je     402077 <seghandler+0x23>
  402061:	48 8d 3d 25 15 00 00 	lea    0x1525(%rip),%rdi        # 40358d <_IO_stdin_used+0x4cd>
  402068:	e8 53 ec ff ff       	callq  400cc0 <puts@plt>
  40206d:	b8 00 00 00 00       	mov    $0x0,%eax
  402072:	e8 e9 fb ff ff       	callq  401c60 <check_fail>
  402077:	48 8d 3d 7a 16 00 00 	lea    0x167a(%rip),%rdi        # 4036f8 <_IO_stdin_used+0x638>
  40207e:	e8 3d ec ff ff       	callq  400cc0 <puts@plt>
  402083:	48 8d 3d ed 14 00 00 	lea    0x14ed(%rip),%rdi        # 403577 <_IO_stdin_used+0x4b7>
  40208a:	e8 31 ec ff ff       	callq  400cc0 <puts@plt>
  40208f:	be 00 00 00 00       	mov    $0x0,%esi
  402094:	bf 00 00 00 00       	mov    $0x0,%edi
  402099:	e8 4c fc ff ff       	callq  401cea <notify_server>
  40209e:	bf 01 00 00 00       	mov    $0x1,%edi
  4020a3:	e8 88 ed ff ff       	callq  400e30 <exit@plt>

00000000004020a8 <illegalhandler>:
  4020a8:	48 83 ec 08          	sub    $0x8,%rsp
  4020ac:	83 3d 55 34 20 00 00 	cmpl   $0x0,0x203455(%rip)        # 605508 <is_checker>
  4020b3:	74 16                	je     4020cb <illegalhandler+0x23>
  4020b5:	48 8d 3d e4 14 00 00 	lea    0x14e4(%rip),%rdi        # 4035a0 <_IO_stdin_used+0x4e0>
  4020bc:	e8 ff eb ff ff       	callq  400cc0 <puts@plt>
  4020c1:	b8 00 00 00 00       	mov    $0x0,%eax
  4020c6:	e8 95 fb ff ff       	callq  401c60 <check_fail>
  4020cb:	48 8d 3d 4e 16 00 00 	lea    0x164e(%rip),%rdi        # 403720 <_IO_stdin_used+0x660>
  4020d2:	e8 e9 eb ff ff       	callq  400cc0 <puts@plt>
  4020d7:	48 8d 3d 99 14 00 00 	lea    0x1499(%rip),%rdi        # 403577 <_IO_stdin_used+0x4b7>
  4020de:	e8 dd eb ff ff       	callq  400cc0 <puts@plt>
  4020e3:	be 00 00 00 00       	mov    $0x0,%esi
  4020e8:	bf 00 00 00 00       	mov    $0x0,%edi
  4020ed:	e8 f8 fb ff ff       	callq  401cea <notify_server>
  4020f2:	bf 01 00 00 00       	mov    $0x1,%edi
  4020f7:	e8 34 ed ff ff       	callq  400e30 <exit@plt>

00000000004020fc <sigalrmhandler>:
  4020fc:	48 83 ec 08          	sub    $0x8,%rsp
  402100:	83 3d 01 34 20 00 00 	cmpl   $0x0,0x203401(%rip)        # 605508 <is_checker>
  402107:	74 16                	je     40211f <sigalrmhandler+0x23>
  402109:	48 8d 3d a4 14 00 00 	lea    0x14a4(%rip),%rdi        # 4035b4 <_IO_stdin_used+0x4f4>
  402110:	e8 ab eb ff ff       	callq  400cc0 <puts@plt>
  402115:	b8 00 00 00 00       	mov    $0x0,%eax
  40211a:	e8 41 fb ff ff       	callq  401c60 <check_fail>
  40211f:	ba 05 00 00 00       	mov    $0x5,%edx
  402124:	48 8d 35 25 16 00 00 	lea    0x1625(%rip),%rsi        # 403750 <_IO_stdin_used+0x690>
  40212b:	bf 01 00 00 00       	mov    $0x1,%edi
  402130:	b8 00 00 00 00       	mov    $0x0,%eax
  402135:	e8 a6 ec ff ff       	callq  400de0 <__printf_chk@plt>
  40213a:	be 00 00 00 00       	mov    $0x0,%esi
  40213f:	bf 00 00 00 00       	mov    $0x0,%edi
  402144:	e8 a1 fb ff ff       	callq  401cea <notify_server>
  402149:	bf 01 00 00 00       	mov    $0x1,%edi
  40214e:	e8 dd ec ff ff       	callq  400e30 <exit@plt>

0000000000402153 <launch>:
  402153:	55                   	push   %rbp
  402154:	48 89 e5             	mov    %rsp,%rbp
  402157:	48 83 ec 10          	sub    $0x10,%rsp
  40215b:	48 89 fa             	mov    %rdi,%rdx
  40215e:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  402165:	00 00 
  402167:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
  40216b:	31 c0                	xor    %eax,%eax
  40216d:	48 8d 47 1e          	lea    0x1e(%rdi),%rax
  402171:	48 83 e0 f0          	and    $0xfffffffffffffff0,%rax
  402175:	48 29 c4             	sub    %rax,%rsp
  402178:	48 8d 7c 24 0f       	lea    0xf(%rsp),%rdi
  40217d:	48 83 e7 f0          	and    $0xfffffffffffffff0,%rdi
  402181:	be f4 00 00 00       	mov    $0xf4,%esi
  402186:	e8 75 eb ff ff       	callq  400d00 <memset@plt>
  40218b:	48 8b 05 0e 33 20 00 	mov    0x20330e(%rip),%rax        # 6054a0 <stdin@@GLIBC_2.2.5>
  402192:	48 39 05 57 33 20 00 	cmp    %rax,0x203357(%rip)        # 6054f0 <infile>
  402199:	74 3a                	je     4021d5 <launch+0x82>
  40219b:	c7 05 57 33 20 00 00 	movl   $0x0,0x203357(%rip)        # 6054fc <vlevel>
  4021a2:	00 00 00 
  4021a5:	b8 00 00 00 00       	mov    $0x0,%eax
  4021aa:	e8 f7 f8 ff ff       	callq  401aa6 <test>
  4021af:	83 3d 52 33 20 00 00 	cmpl   $0x0,0x203352(%rip)        # 605508 <is_checker>
  4021b6:	75 35                	jne    4021ed <launch+0x9a>
  4021b8:	48 8d 3d 15 14 00 00 	lea    0x1415(%rip),%rdi        # 4035d4 <_IO_stdin_used+0x514>
  4021bf:	e8 fc ea ff ff       	callq  400cc0 <puts@plt>
  4021c4:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4021c8:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
  4021cf:	00 00 
  4021d1:	75 30                	jne    402203 <launch+0xb0>
  4021d3:	c9                   	leaveq 
  4021d4:	c3                   	retq   
  4021d5:	48 8d 35 e0 13 00 00 	lea    0x13e0(%rip),%rsi        # 4035bc <_IO_stdin_used+0x4fc>
  4021dc:	bf 01 00 00 00       	mov    $0x1,%edi
  4021e1:	b8 00 00 00 00       	mov    $0x0,%eax
  4021e6:	e8 f5 eb ff ff       	callq  400de0 <__printf_chk@plt>
  4021eb:	eb ae                	jmp    40219b <launch+0x48>
  4021ed:	48 8d 3d d5 13 00 00 	lea    0x13d5(%rip),%rdi        # 4035c9 <_IO_stdin_used+0x509>
  4021f4:	e8 c7 ea ff ff       	callq  400cc0 <puts@plt>
  4021f9:	b8 00 00 00 00       	mov    $0x0,%eax
  4021fe:	e8 5d fa ff ff       	callq  401c60 <check_fail>
  402203:	e8 d8 ea ff ff       	callq  400ce0 <__stack_chk_fail@plt>

0000000000402208 <stable_launch>:
  402208:	53                   	push   %rbx
  402209:	48 89 3d d8 32 20 00 	mov    %rdi,0x2032d8(%rip)        # 6054e8 <global_offset>
  402210:	41 b9 00 00 00 00    	mov    $0x0,%r9d
  402216:	41 b8 00 00 00 00    	mov    $0x0,%r8d
  40221c:	b9 32 01 00 00       	mov    $0x132,%ecx
  402221:	ba 07 00 00 00       	mov    $0x7,%edx
  402226:	be 00 00 10 00       	mov    $0x100000,%esi
  40222b:	bf 00 60 58 55       	mov    $0x55586000,%edi
  402230:	e8 bb ea ff ff       	callq  400cf0 <mmap@plt>
  402235:	48 89 c3             	mov    %rax,%rbx
  402238:	48 3d 00 60 58 55    	cmp    $0x55586000,%rax
  40223e:	75 43                	jne    402283 <stable_launch+0x7b>
  402240:	48 8d 90 f8 ff 0f 00 	lea    0xffff8(%rax),%rdx
  402247:	48 89 15 e2 3e 20 00 	mov    %rdx,0x203ee2(%rip)        # 606130 <stack_top>
  40224e:	48 89 e0             	mov    %rsp,%rax
  402251:	48 89 d4             	mov    %rdx,%rsp
  402254:	48 89 c2             	mov    %rax,%rdx
  402257:	48 89 15 82 32 20 00 	mov    %rdx,0x203282(%rip)        # 6054e0 <global_save_stack>
  40225e:	48 8b 3d 83 32 20 00 	mov    0x203283(%rip),%rdi        # 6054e8 <global_offset>
  402265:	e8 e9 fe ff ff       	callq  402153 <launch>
  40226a:	48 8b 05 6f 32 20 00 	mov    0x20326f(%rip),%rax        # 6054e0 <global_save_stack>
  402271:	48 89 c4             	mov    %rax,%rsp
  402274:	be 00 00 10 00       	mov    $0x100000,%esi
  402279:	48 89 df             	mov    %rbx,%rdi
  40227c:	e8 4f eb ff ff       	callq  400dd0 <munmap@plt>
  402281:	5b                   	pop    %rbx
  402282:	c3                   	retq   
  402283:	be 00 00 10 00       	mov    $0x100000,%esi
  402288:	48 89 c7             	mov    %rax,%rdi
  40228b:	e8 40 eb ff ff       	callq  400dd0 <munmap@plt>
  402290:	b9 00 60 58 55       	mov    $0x55586000,%ecx
  402295:	48 8d 15 ec 14 00 00 	lea    0x14ec(%rip),%rdx        # 403788 <_IO_stdin_used+0x6c8>
  40229c:	be 01 00 00 00       	mov    $0x1,%esi
  4022a1:	48 8b 3d 18 32 20 00 	mov    0x203218(%rip),%rdi        # 6054c0 <stderr@@GLIBC_2.2.5>
  4022a8:	b8 00 00 00 00       	mov    $0x0,%eax
  4022ad:	e8 9e eb ff ff       	callq  400e50 <__fprintf_chk@plt>
  4022b2:	bf 01 00 00 00       	mov    $0x1,%edi
  4022b7:	e8 74 eb ff ff       	callq  400e30 <exit@plt>

00000000004022bc <rio_readinitb>:
  4022bc:	89 37                	mov    %esi,(%rdi)
  4022be:	c7 47 04 00 00 00 00 	movl   $0x0,0x4(%rdi)
  4022c5:	48 8d 47 10          	lea    0x10(%rdi),%rax
  4022c9:	48 89 47 08          	mov    %rax,0x8(%rdi)
  4022cd:	c3                   	retq   

00000000004022ce <sigalrm_handler>:
  4022ce:	48 83 ec 08          	sub    $0x8,%rsp
  4022d2:	b9 00 00 00 00       	mov    $0x0,%ecx
  4022d7:	48 8d 15 e2 14 00 00 	lea    0x14e2(%rip),%rdx        # 4037c0 <trans_char+0x10>
  4022de:	be 01 00 00 00       	mov    $0x1,%esi
  4022e3:	48 8b 3d d6 31 20 00 	mov    0x2031d6(%rip),%rdi        # 6054c0 <stderr@@GLIBC_2.2.5>
  4022ea:	b8 00 00 00 00       	mov    $0x0,%eax
  4022ef:	e8 5c eb ff ff       	callq  400e50 <__fprintf_chk@plt>
  4022f4:	bf 01 00 00 00       	mov    $0x1,%edi
  4022f9:	e8 32 eb ff ff       	callq  400e30 <exit@plt>

00000000004022fe <rio_writen>:
  4022fe:	41 55                	push   %r13
  402300:	41 54                	push   %r12
  402302:	55                   	push   %rbp
  402303:	53                   	push   %rbx
  402304:	48 83 ec 08          	sub    $0x8,%rsp
  402308:	41 89 fc             	mov    %edi,%r12d
  40230b:	48 89 f5             	mov    %rsi,%rbp
  40230e:	49 89 d5             	mov    %rdx,%r13
  402311:	48 89 d3             	mov    %rdx,%rbx
  402314:	eb 06                	jmp    40231c <rio_writen+0x1e>
  402316:	48 29 c3             	sub    %rax,%rbx
  402319:	48 01 c5             	add    %rax,%rbp
  40231c:	48 85 db             	test   %rbx,%rbx
  40231f:	74 24                	je     402345 <rio_writen+0x47>
  402321:	48 89 da             	mov    %rbx,%rdx
  402324:	48 89 ee             	mov    %rbp,%rsi
  402327:	44 89 e7             	mov    %r12d,%edi
  40232a:	e8 a1 e9 ff ff       	callq  400cd0 <write@plt>
  40232f:	48 85 c0             	test   %rax,%rax
  402332:	7f e2                	jg     402316 <rio_writen+0x18>
  402334:	e8 47 e9 ff ff       	callq  400c80 <__errno_location@plt>
  402339:	83 38 04             	cmpl   $0x4,(%rax)
  40233c:	75 15                	jne    402353 <rio_writen+0x55>
  40233e:	b8 00 00 00 00       	mov    $0x0,%eax
  402343:	eb d1                	jmp    402316 <rio_writen+0x18>
  402345:	4c 89 e8             	mov    %r13,%rax
  402348:	48 83 c4 08          	add    $0x8,%rsp
  40234c:	5b                   	pop    %rbx
  40234d:	5d                   	pop    %rbp
  40234e:	41 5c                	pop    %r12
  402350:	41 5d                	pop    %r13
  402352:	c3                   	retq   
  402353:	48 c7 c0 ff ff ff ff 	mov    $0xffffffffffffffff,%rax
  40235a:	eb ec                	jmp    402348 <rio_writen+0x4a>

000000000040235c <rio_read>:
  40235c:	41 55                	push   %r13
  40235e:	41 54                	push   %r12
  402360:	55                   	push   %rbp
  402361:	53                   	push   %rbx
  402362:	48 83 ec 08          	sub    $0x8,%rsp
  402366:	48 89 fb             	mov    %rdi,%rbx
  402369:	49 89 f5             	mov    %rsi,%r13
  40236c:	49 89 d4             	mov    %rdx,%r12
  40236f:	eb 0a                	jmp    40237b <rio_read+0x1f>
  402371:	e8 0a e9 ff ff       	callq  400c80 <__errno_location@plt>
  402376:	83 38 04             	cmpl   $0x4,(%rax)
  402379:	75 5c                	jne    4023d7 <rio_read+0x7b>
  40237b:	8b 6b 04             	mov    0x4(%rbx),%ebp
  40237e:	85 ed                	test   %ebp,%ebp
  402380:	7f 24                	jg     4023a6 <rio_read+0x4a>
  402382:	48 8d 6b 10          	lea    0x10(%rbx),%rbp
  402386:	8b 3b                	mov    (%rbx),%edi
  402388:	ba 00 20 00 00       	mov    $0x2000,%edx
  40238d:	48 89 ee             	mov    %rbp,%rsi
  402390:	e8 9b e9 ff ff       	callq  400d30 <read@plt>
  402395:	89 43 04             	mov    %eax,0x4(%rbx)
  402398:	85 c0                	test   %eax,%eax
  40239a:	78 d5                	js     402371 <rio_read+0x15>
  40239c:	85 c0                	test   %eax,%eax
  40239e:	74 40                	je     4023e0 <rio_read+0x84>
  4023a0:	48 89 6b 08          	mov    %rbp,0x8(%rbx)
  4023a4:	eb d5                	jmp    40237b <rio_read+0x1f>
  4023a6:	89 e8                	mov    %ebp,%eax
  4023a8:	4c 39 e0             	cmp    %r12,%rax
  4023ab:	72 03                	jb     4023b0 <rio_read+0x54>
  4023ad:	44 89 e5             	mov    %r12d,%ebp
  4023b0:	4c 63 e5             	movslq %ebp,%r12
  4023b3:	48 8b 73 08          	mov    0x8(%rbx),%rsi
  4023b7:	4c 89 e2             	mov    %r12,%rdx
  4023ba:	4c 89 ef             	mov    %r13,%rdi
  4023bd:	e8 be e9 ff ff       	callq  400d80 <memcpy@plt>
  4023c2:	4c 01 63 08          	add    %r12,0x8(%rbx)
  4023c6:	29 6b 04             	sub    %ebp,0x4(%rbx)
  4023c9:	4c 89 e0             	mov    %r12,%rax
  4023cc:	48 83 c4 08          	add    $0x8,%rsp
  4023d0:	5b                   	pop    %rbx
  4023d1:	5d                   	pop    %rbp
  4023d2:	41 5c                	pop    %r12
  4023d4:	41 5d                	pop    %r13
  4023d6:	c3                   	retq   
  4023d7:	48 c7 c0 ff ff ff ff 	mov    $0xffffffffffffffff,%rax
  4023de:	eb ec                	jmp    4023cc <rio_read+0x70>
  4023e0:	b8 00 00 00 00       	mov    $0x0,%eax
  4023e5:	eb e5                	jmp    4023cc <rio_read+0x70>

00000000004023e7 <rio_readlineb>:
  4023e7:	41 55                	push   %r13
  4023e9:	41 54                	push   %r12
  4023eb:	55                   	push   %rbp
  4023ec:	53                   	push   %rbx
  4023ed:	48 83 ec 18          	sub    $0x18,%rsp
  4023f1:	49 89 fd             	mov    %rdi,%r13
  4023f4:	48 89 f5             	mov    %rsi,%rbp
  4023f7:	49 89 d4             	mov    %rdx,%r12
  4023fa:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  402401:	00 00 
  402403:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
  402408:	31 c0                	xor    %eax,%eax
  40240a:	bb 01 00 00 00       	mov    $0x1,%ebx
  40240f:	4c 39 e3             	cmp    %r12,%rbx
  402412:	73 47                	jae    40245b <rio_readlineb+0x74>
  402414:	48 8d 74 24 07       	lea    0x7(%rsp),%rsi
  402419:	ba 01 00 00 00       	mov    $0x1,%edx
  40241e:	4c 89 ef             	mov    %r13,%rdi
  402421:	e8 36 ff ff ff       	callq  40235c <rio_read>
  402426:	83 f8 01             	cmp    $0x1,%eax
  402429:	75 1c                	jne    402447 <rio_readlineb+0x60>
  40242b:	48 8d 45 01          	lea    0x1(%rbp),%rax
  40242f:	0f b6 54 24 07       	movzbl 0x7(%rsp),%edx
  402434:	88 55 00             	mov    %dl,0x0(%rbp)
  402437:	80 7c 24 07 0a       	cmpb   $0xa,0x7(%rsp)
  40243c:	74 1a                	je     402458 <rio_readlineb+0x71>
  40243e:	48 83 c3 01          	add    $0x1,%rbx
  402442:	48 89 c5             	mov    %rax,%rbp
  402445:	eb c8                	jmp    40240f <rio_readlineb+0x28>
  402447:	85 c0                	test   %eax,%eax
  402449:	75 32                	jne    40247d <rio_readlineb+0x96>
  40244b:	48 83 fb 01          	cmp    $0x1,%rbx
  40244f:	75 0a                	jne    40245b <rio_readlineb+0x74>
  402451:	b8 00 00 00 00       	mov    $0x0,%eax
  402456:	eb 0a                	jmp    402462 <rio_readlineb+0x7b>
  402458:	48 89 c5             	mov    %rax,%rbp
  40245b:	c6 45 00 00          	movb   $0x0,0x0(%rbp)
  40245f:	48 89 d8             	mov    %rbx,%rax
  402462:	48 8b 4c 24 08       	mov    0x8(%rsp),%rcx
  402467:	64 48 33 0c 25 28 00 	xor    %fs:0x28,%rcx
  40246e:	00 00 
  402470:	75 14                	jne    402486 <rio_readlineb+0x9f>
  402472:	48 83 c4 18          	add    $0x18,%rsp
  402476:	5b                   	pop    %rbx
  402477:	5d                   	pop    %rbp
  402478:	41 5c                	pop    %r12
  40247a:	41 5d                	pop    %r13
  40247c:	c3                   	retq   
  40247d:	48 c7 c0 ff ff ff ff 	mov    $0xffffffffffffffff,%rax
  402484:	eb dc                	jmp    402462 <rio_readlineb+0x7b>
  402486:	e8 55 e8 ff ff       	callq  400ce0 <__stack_chk_fail@plt>

000000000040248b <urlencode>:
  40248b:	41 54                	push   %r12
  40248d:	55                   	push   %rbp
  40248e:	53                   	push   %rbx
  40248f:	48 83 ec 10          	sub    $0x10,%rsp
  402493:	48 89 fb             	mov    %rdi,%rbx
  402496:	48 89 f5             	mov    %rsi,%rbp
  402499:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  4024a0:	00 00 
  4024a2:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
  4024a7:	31 c0                	xor    %eax,%eax
  4024a9:	48 c7 c1 ff ff ff ff 	mov    $0xffffffffffffffff,%rcx
  4024b0:	f2 ae                	repnz scas %es:(%rdi),%al
  4024b2:	48 89 ce             	mov    %rcx,%rsi
  4024b5:	48 f7 d6             	not    %rsi
  4024b8:	8d 46 ff             	lea    -0x1(%rsi),%eax
  4024bb:	eb 0f                	jmp    4024cc <urlencode+0x41>
  4024bd:	44 88 45 00          	mov    %r8b,0x0(%rbp)
  4024c1:	48 8d 6d 01          	lea    0x1(%rbp),%rbp
  4024c5:	48 83 c3 01          	add    $0x1,%rbx
  4024c9:	44 89 e0             	mov    %r12d,%eax
  4024cc:	44 8d 60 ff          	lea    -0x1(%rax),%r12d
  4024d0:	85 c0                	test   %eax,%eax
  4024d2:	0f 84 a8 00 00 00    	je     402580 <urlencode+0xf5>
  4024d8:	44 0f b6 03          	movzbl (%rbx),%r8d
  4024dc:	41 80 f8 2a          	cmp    $0x2a,%r8b
  4024e0:	0f 94 c2             	sete   %dl
  4024e3:	41 80 f8 2d          	cmp    $0x2d,%r8b
  4024e7:	0f 94 c0             	sete   %al
  4024ea:	08 c2                	or     %al,%dl
  4024ec:	75 cf                	jne    4024bd <urlencode+0x32>
  4024ee:	41 80 f8 2e          	cmp    $0x2e,%r8b
  4024f2:	74 c9                	je     4024bd <urlencode+0x32>
  4024f4:	41 80 f8 5f          	cmp    $0x5f,%r8b
  4024f8:	74 c3                	je     4024bd <urlencode+0x32>
  4024fa:	41 8d 40 d0          	lea    -0x30(%r8),%eax
  4024fe:	3c 09                	cmp    $0x9,%al
  402500:	76 bb                	jbe    4024bd <urlencode+0x32>
  402502:	41 8d 40 bf          	lea    -0x41(%r8),%eax
  402506:	3c 19                	cmp    $0x19,%al
  402508:	76 b3                	jbe    4024bd <urlencode+0x32>
  40250a:	41 8d 40 9f          	lea    -0x61(%r8),%eax
  40250e:	3c 19                	cmp    $0x19,%al
  402510:	76 ab                	jbe    4024bd <urlencode+0x32>
  402512:	41 80 f8 20          	cmp    $0x20,%r8b
  402516:	74 56                	je     40256e <urlencode+0xe3>
  402518:	41 8d 40 e0          	lea    -0x20(%r8),%eax
  40251c:	3c 5f                	cmp    $0x5f,%al
  40251e:	0f 96 c2             	setbe  %dl
  402521:	41 80 f8 09          	cmp    $0x9,%r8b
  402525:	0f 94 c0             	sete   %al
  402528:	08 c2                	or     %al,%dl
  40252a:	74 4f                	je     40257b <urlencode+0xf0>
  40252c:	48 89 e7             	mov    %rsp,%rdi
  40252f:	45 0f b6 c0          	movzbl %r8b,%r8d
  402533:	48 8d 0d 1e 13 00 00 	lea    0x131e(%rip),%rcx        # 403858 <trans_char+0xa8>
  40253a:	ba 08 00 00 00       	mov    $0x8,%edx
  40253f:	be 01 00 00 00       	mov    $0x1,%esi
  402544:	b8 00 00 00 00       	mov    $0x0,%eax
  402549:	e8 12 e9 ff ff       	callq  400e60 <__sprintf_chk@plt>
  40254e:	0f b6 04 24          	movzbl (%rsp),%eax
  402552:	88 45 00             	mov    %al,0x0(%rbp)
  402555:	0f b6 44 24 01       	movzbl 0x1(%rsp),%eax
  40255a:	88 45 01             	mov    %al,0x1(%rbp)
  40255d:	0f b6 44 24 02       	movzbl 0x2(%rsp),%eax
  402562:	88 45 02             	mov    %al,0x2(%rbp)
  402565:	48 8d 6d 03          	lea    0x3(%rbp),%rbp
  402569:	e9 57 ff ff ff       	jmpq   4024c5 <urlencode+0x3a>
  40256e:	c6 45 00 2b          	movb   $0x2b,0x0(%rbp)
  402572:	48 8d 6d 01          	lea    0x1(%rbp),%rbp
  402576:	e9 4a ff ff ff       	jmpq   4024c5 <urlencode+0x3a>
  40257b:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402580:	48 8b 74 24 08       	mov    0x8(%rsp),%rsi
  402585:	64 48 33 34 25 28 00 	xor    %fs:0x28,%rsi
  40258c:	00 00 
  40258e:	75 09                	jne    402599 <urlencode+0x10e>
  402590:	48 83 c4 10          	add    $0x10,%rsp
  402594:	5b                   	pop    %rbx
  402595:	5d                   	pop    %rbp
  402596:	41 5c                	pop    %r12
  402598:	c3                   	retq   
  402599:	e8 42 e7 ff ff       	callq  400ce0 <__stack_chk_fail@plt>

000000000040259e <submitr>:
  40259e:	41 57                	push   %r15
  4025a0:	41 56                	push   %r14
  4025a2:	41 55                	push   %r13
  4025a4:	41 54                	push   %r12
  4025a6:	55                   	push   %rbp
  4025a7:	53                   	push   %rbx
  4025a8:	48 81 ec 68 a0 00 00 	sub    $0xa068,%rsp
  4025af:	49 89 fd             	mov    %rdi,%r13
  4025b2:	89 74 24 14          	mov    %esi,0x14(%rsp)
  4025b6:	49 89 d7             	mov    %rdx,%r15
  4025b9:	48 89 4c 24 08       	mov    %rcx,0x8(%rsp)
  4025be:	4c 89 44 24 18       	mov    %r8,0x18(%rsp)
  4025c3:	4d 89 ce             	mov    %r9,%r14
  4025c6:	48 8b ac 24 a0 a0 00 	mov    0xa0a0(%rsp),%rbp
  4025cd:	00 
  4025ce:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  4025d5:	00 00 
  4025d7:	48 89 84 24 58 a0 00 	mov    %rax,0xa058(%rsp)
  4025de:	00 
  4025df:	31 c0                	xor    %eax,%eax
  4025e1:	c7 44 24 2c 00 00 00 	movl   $0x0,0x2c(%rsp)
  4025e8:	00 
  4025e9:	ba 00 00 00 00       	mov    $0x0,%edx
  4025ee:	be 01 00 00 00       	mov    $0x1,%esi
  4025f3:	bf 02 00 00 00       	mov    $0x2,%edi
  4025f8:	e8 73 e8 ff ff       	callq  400e70 <socket@plt>
  4025fd:	85 c0                	test   %eax,%eax
  4025ff:	0f 88 a9 02 00 00    	js     4028ae <submitr+0x310>
  402605:	89 c3                	mov    %eax,%ebx
  402607:	4c 89 ef             	mov    %r13,%rdi
  40260a:	e8 41 e7 ff ff       	callq  400d50 <gethostbyname@plt>
  40260f:	48 85 c0             	test   %rax,%rax
  402612:	0f 84 e2 02 00 00    	je     4028fa <submitr+0x35c>
  402618:	4c 8d 64 24 30       	lea    0x30(%rsp),%r12
  40261d:	48 c7 44 24 32 00 00 	movq   $0x0,0x32(%rsp)
  402624:	00 00 
  402626:	c7 44 24 3a 00 00 00 	movl   $0x0,0x3a(%rsp)
  40262d:	00 
  40262e:	66 c7 44 24 3e 00 00 	movw   $0x0,0x3e(%rsp)
  402635:	66 c7 44 24 30 02 00 	movw   $0x2,0x30(%rsp)
  40263c:	48 63 50 14          	movslq 0x14(%rax),%rdx
  402640:	48 8b 40 18          	mov    0x18(%rax),%rax
  402644:	48 8b 30             	mov    (%rax),%rsi
  402647:	48 8d 7c 24 34       	lea    0x34(%rsp),%rdi
  40264c:	b9 0c 00 00 00       	mov    $0xc,%ecx
  402651:	e8 0a e7 ff ff       	callq  400d60 <__memmove_chk@plt>
  402656:	0f b7 44 24 14       	movzwl 0x14(%rsp),%eax
  40265b:	66 c1 c8 08          	ror    $0x8,%ax
  40265f:	66 89 44 24 32       	mov    %ax,0x32(%rsp)
  402664:	ba 10 00 00 00       	mov    $0x10,%edx
  402669:	4c 89 e6             	mov    %r12,%rsi
  40266c:	89 df                	mov    %ebx,%edi
  40266e:	e8 cd e7 ff ff       	callq  400e40 <connect@plt>
  402673:	85 c0                	test   %eax,%eax
  402675:	0f 88 e7 02 00 00    	js     402962 <submitr+0x3c4>
  40267b:	48 c7 c6 ff ff ff ff 	mov    $0xffffffffffffffff,%rsi
  402682:	b8 00 00 00 00       	mov    $0x0,%eax
  402687:	48 89 f1             	mov    %rsi,%rcx
  40268a:	4c 89 f7             	mov    %r14,%rdi
  40268d:	f2 ae                	repnz scas %es:(%rdi),%al
  40268f:	48 89 ca             	mov    %rcx,%rdx
  402692:	48 f7 d2             	not    %rdx
  402695:	48 89 f1             	mov    %rsi,%rcx
  402698:	4c 89 ff             	mov    %r15,%rdi
  40269b:	f2 ae                	repnz scas %es:(%rdi),%al
  40269d:	48 f7 d1             	not    %rcx
  4026a0:	49 89 c8             	mov    %rcx,%r8
  4026a3:	48 89 f1             	mov    %rsi,%rcx
  4026a6:	48 8b 7c 24 08       	mov    0x8(%rsp),%rdi
  4026ab:	f2 ae                	repnz scas %es:(%rdi),%al
  4026ad:	48 f7 d1             	not    %rcx
  4026b0:	4d 8d 44 08 fe       	lea    -0x2(%r8,%rcx,1),%r8
  4026b5:	48 89 f1             	mov    %rsi,%rcx
  4026b8:	48 8b 7c 24 18       	mov    0x18(%rsp),%rdi
  4026bd:	f2 ae                	repnz scas %es:(%rdi),%al
  4026bf:	48 89 c8             	mov    %rcx,%rax
  4026c2:	48 f7 d0             	not    %rax
  4026c5:	49 8d 4c 00 ff       	lea    -0x1(%r8,%rax,1),%rcx
  4026ca:	48 8d 44 52 fd       	lea    -0x3(%rdx,%rdx,2),%rax
  4026cf:	48 8d 84 01 80 00 00 	lea    0x80(%rcx,%rax,1),%rax
  4026d6:	00 
  4026d7:	48 3d 00 20 00 00    	cmp    $0x2000,%rax
  4026dd:	0f 87 d9 02 00 00    	ja     4029bc <submitr+0x41e>
  4026e3:	48 8d b4 24 50 40 00 	lea    0x4050(%rsp),%rsi
  4026ea:	00 
  4026eb:	b9 00 04 00 00       	mov    $0x400,%ecx
  4026f0:	b8 00 00 00 00       	mov    $0x0,%eax
  4026f5:	48 89 f7             	mov    %rsi,%rdi
  4026f8:	f3 48 ab             	rep stos %rax,%es:(%rdi)
  4026fb:	4c 89 f7             	mov    %r14,%rdi
  4026fe:	e8 88 fd ff ff       	callq  40248b <urlencode>
  402703:	85 c0                	test   %eax,%eax
  402705:	0f 88 24 03 00 00    	js     402a2f <submitr+0x491>
  40270b:	4c 8d a4 24 50 20 00 	lea    0x2050(%rsp),%r12
  402712:	00 
  402713:	41 55                	push   %r13
  402715:	48 8d 84 24 58 40 00 	lea    0x4058(%rsp),%rax
  40271c:	00 
  40271d:	50                   	push   %rax
  40271e:	4d 89 f9             	mov    %r15,%r9
  402721:	4c 8b 44 24 18       	mov    0x18(%rsp),%r8
  402726:	48 8d 0d bb 10 00 00 	lea    0x10bb(%rip),%rcx        # 4037e8 <trans_char+0x38>
  40272d:	ba 00 20 00 00       	mov    $0x2000,%edx
  402732:	be 01 00 00 00       	mov    $0x1,%esi
  402737:	4c 89 e7             	mov    %r12,%rdi
  40273a:	b8 00 00 00 00       	mov    $0x0,%eax
  40273f:	e8 1c e7 ff ff       	callq  400e60 <__sprintf_chk@plt>
  402744:	48 c7 c1 ff ff ff ff 	mov    $0xffffffffffffffff,%rcx
  40274b:	b8 00 00 00 00       	mov    $0x0,%eax
  402750:	4c 89 e7             	mov    %r12,%rdi
  402753:	f2 ae                	repnz scas %es:(%rdi),%al
  402755:	48 89 ca             	mov    %rcx,%rdx
  402758:	48 f7 d2             	not    %rdx
  40275b:	48 8d 52 ff          	lea    -0x1(%rdx),%rdx
  40275f:	4c 89 e6             	mov    %r12,%rsi
  402762:	89 df                	mov    %ebx,%edi
  402764:	e8 95 fb ff ff       	callq  4022fe <rio_writen>
  402769:	48 83 c4 10          	add    $0x10,%rsp
  40276d:	48 85 c0             	test   %rax,%rax
  402770:	0f 88 44 03 00 00    	js     402aba <submitr+0x51c>
  402776:	4c 8d 64 24 40       	lea    0x40(%rsp),%r12
  40277b:	89 de                	mov    %ebx,%esi
  40277d:	4c 89 e7             	mov    %r12,%rdi
  402780:	e8 37 fb ff ff       	callq  4022bc <rio_readinitb>
  402785:	48 8d b4 24 50 20 00 	lea    0x2050(%rsp),%rsi
  40278c:	00 
  40278d:	ba 00 20 00 00       	mov    $0x2000,%edx
  402792:	4c 89 e7             	mov    %r12,%rdi
  402795:	e8 4d fc ff ff       	callq  4023e7 <rio_readlineb>
  40279a:	48 85 c0             	test   %rax,%rax
  40279d:	0f 8e 86 03 00 00    	jle    402b29 <submitr+0x58b>
  4027a3:	48 8d 4c 24 2c       	lea    0x2c(%rsp),%rcx
  4027a8:	48 8d 94 24 50 60 00 	lea    0x6050(%rsp),%rdx
  4027af:	00 
  4027b0:	48 8d bc 24 50 20 00 	lea    0x2050(%rsp),%rdi
  4027b7:	00 
  4027b8:	4c 8d 84 24 50 80 00 	lea    0x8050(%rsp),%r8
  4027bf:	00 
  4027c0:	48 8d 35 98 10 00 00 	lea    0x1098(%rip),%rsi        # 40385f <trans_char+0xaf>
  4027c7:	b8 00 00 00 00       	mov    $0x0,%eax
  4027cc:	e8 ef e5 ff ff       	callq  400dc0 <__isoc99_sscanf@plt>
  4027d1:	48 8d b4 24 50 20 00 	lea    0x2050(%rsp),%rsi
  4027d8:	00 
  4027d9:	b9 03 00 00 00       	mov    $0x3,%ecx
  4027de:	48 8d 3d 91 10 00 00 	lea    0x1091(%rip),%rdi        # 403876 <trans_char+0xc6>
  4027e5:	f3 a6                	repz cmpsb %es:(%rdi),%ds:(%rsi)
  4027e7:	0f 97 c0             	seta   %al
  4027ea:	1c 00                	sbb    $0x0,%al
  4027ec:	84 c0                	test   %al,%al
  4027ee:	0f 84 b3 03 00 00    	je     402ba7 <submitr+0x609>
  4027f4:	48 8d b4 24 50 20 00 	lea    0x2050(%rsp),%rsi
  4027fb:	00 
  4027fc:	48 8d 7c 24 40       	lea    0x40(%rsp),%rdi
  402801:	ba 00 20 00 00       	mov    $0x2000,%edx
  402806:	e8 dc fb ff ff       	callq  4023e7 <rio_readlineb>
  40280b:	48 85 c0             	test   %rax,%rax
  40280e:	7f c1                	jg     4027d1 <submitr+0x233>
  402810:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
  402817:	3a 20 43 
  40281a:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
  402821:	20 75 6e 
  402824:	48 89 45 00          	mov    %rax,0x0(%rbp)
  402828:	48 89 55 08          	mov    %rdx,0x8(%rbp)
  40282c:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
  402833:	74 6f 20 
  402836:	48 ba 72 65 61 64 20 	movabs $0x6165682064616572,%rdx
  40283d:	68 65 61 
  402840:	48 89 45 10          	mov    %rax,0x10(%rbp)
  402844:	48 89 55 18          	mov    %rdx,0x18(%rbp)
  402848:	48 b8 64 65 72 73 20 	movabs $0x6f72662073726564,%rax
  40284f:	66 72 6f 
  402852:	48 ba 6d 20 74 68 65 	movabs $0x657220656874206d,%rdx
  402859:	20 72 65 
  40285c:	48 89 45 20          	mov    %rax,0x20(%rbp)
  402860:	48 89 55 28          	mov    %rdx,0x28(%rbp)
  402864:	48 b8 73 75 6c 74 20 	movabs $0x72657320746c7573,%rax
  40286b:	73 65 72 
  40286e:	48 89 45 30          	mov    %rax,0x30(%rbp)
  402872:	c7 45 38 76 65 72 00 	movl   $0x726576,0x38(%rbp)
  402879:	89 df                	mov    %ebx,%edi
  40287b:	e8 a0 e4 ff ff       	callq  400d20 <close@plt>
  402880:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402885:	48 8b 9c 24 58 a0 00 	mov    0xa058(%rsp),%rbx
  40288c:	00 
  40288d:	64 48 33 1c 25 28 00 	xor    %fs:0x28,%rbx
  402894:	00 00 
  402896:	0f 85 7e 04 00 00    	jne    402d1a <submitr+0x77c>
  40289c:	48 81 c4 68 a0 00 00 	add    $0xa068,%rsp
  4028a3:	5b                   	pop    %rbx
  4028a4:	5d                   	pop    %rbp
  4028a5:	41 5c                	pop    %r12
  4028a7:	41 5d                	pop    %r13
  4028a9:	41 5e                	pop    %r14
  4028ab:	41 5f                	pop    %r15
  4028ad:	c3                   	retq   
  4028ae:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
  4028b5:	3a 20 43 
  4028b8:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
  4028bf:	20 75 6e 
  4028c2:	48 89 45 00          	mov    %rax,0x0(%rbp)
  4028c6:	48 89 55 08          	mov    %rdx,0x8(%rbp)
  4028ca:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
  4028d1:	74 6f 20 
  4028d4:	48 ba 63 72 65 61 74 	movabs $0x7320657461657263,%rdx
  4028db:	65 20 73 
  4028de:	48 89 45 10          	mov    %rax,0x10(%rbp)
  4028e2:	48 89 55 18          	mov    %rdx,0x18(%rbp)
  4028e6:	c7 45 20 6f 63 6b 65 	movl   $0x656b636f,0x20(%rbp)
  4028ed:	66 c7 45 24 74 00    	movw   $0x74,0x24(%rbp)
  4028f3:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  4028f8:	eb 8b                	jmp    402885 <submitr+0x2e7>
  4028fa:	48 b8 45 72 72 6f 72 	movabs $0x44203a726f727245,%rax
  402901:	3a 20 44 
  402904:	48 ba 4e 53 20 69 73 	movabs $0x6e7520736920534e,%rdx
  40290b:	20 75 6e 
  40290e:	48 89 45 00          	mov    %rax,0x0(%rbp)
  402912:	48 89 55 08          	mov    %rdx,0x8(%rbp)
  402916:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
  40291d:	74 6f 20 
  402920:	48 ba 72 65 73 6f 6c 	movabs $0x2065766c6f736572,%rdx
  402927:	76 65 20 
  40292a:	48 89 45 10          	mov    %rax,0x10(%rbp)
  40292e:	48 89 55 18          	mov    %rdx,0x18(%rbp)
  402932:	48 b8 73 65 72 76 65 	movabs $0x6120726576726573,%rax
  402939:	72 20 61 
  40293c:	48 89 45 20          	mov    %rax,0x20(%rbp)
  402940:	c7 45 28 64 64 72 65 	movl   $0x65726464,0x28(%rbp)
  402947:	66 c7 45 2c 73 73    	movw   $0x7373,0x2c(%rbp)
  40294d:	c6 45 2e 00          	movb   $0x0,0x2e(%rbp)
  402951:	89 df                	mov    %ebx,%edi
  402953:	e8 c8 e3 ff ff       	callq  400d20 <close@plt>
  402958:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  40295d:	e9 23 ff ff ff       	jmpq   402885 <submitr+0x2e7>
  402962:	48 b8 45 72 72 6f 72 	movabs $0x55203a726f727245,%rax
  402969:	3a 20 55 
  40296c:	48 ba 6e 61 62 6c 65 	movabs $0x6f7420656c62616e,%rdx
  402973:	20 74 6f 
  402976:	48 89 45 00          	mov    %rax,0x0(%rbp)
  40297a:	48 89 55 08          	mov    %rdx,0x8(%rbp)
  40297e:	48 b8 20 63 6f 6e 6e 	movabs $0x7463656e6e6f6320,%rax
  402985:	65 63 74 
  402988:	48 ba 20 74 6f 20 74 	movabs $0x20656874206f7420,%rdx
  40298f:	68 65 20 
  402992:	48 89 45 10          	mov    %rax,0x10(%rbp)
  402996:	48 89 55 18          	mov    %rdx,0x18(%rbp)
  40299a:	c7 45 20 73 65 72 76 	movl   $0x76726573,0x20(%rbp)
  4029a1:	66 c7 45 24 65 72    	movw   $0x7265,0x24(%rbp)
  4029a7:	c6 45 26 00          	movb   $0x0,0x26(%rbp)
  4029ab:	89 df                	mov    %ebx,%edi
  4029ad:	e8 6e e3 ff ff       	callq  400d20 <close@plt>
  4029b2:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  4029b7:	e9 c9 fe ff ff       	jmpq   402885 <submitr+0x2e7>
  4029bc:	48 b8 45 72 72 6f 72 	movabs $0x52203a726f727245,%rax
  4029c3:	3a 20 52 
  4029c6:	48 ba 65 73 75 6c 74 	movabs $0x747320746c757365,%rdx
  4029cd:	20 73 74 
  4029d0:	48 89 45 00          	mov    %rax,0x0(%rbp)
  4029d4:	48 89 55 08          	mov    %rdx,0x8(%rbp)
  4029d8:	48 b8 72 69 6e 67 20 	movabs $0x6f6f7420676e6972,%rax
  4029df:	74 6f 6f 
  4029e2:	48 ba 20 6c 61 72 67 	movabs $0x202e656772616c20,%rdx
  4029e9:	65 2e 20 
  4029ec:	48 89 45 10          	mov    %rax,0x10(%rbp)
  4029f0:	48 89 55 18          	mov    %rdx,0x18(%rbp)
  4029f4:	48 b8 49 6e 63 72 65 	movabs $0x6573616572636e49,%rax
  4029fb:	61 73 65 
  4029fe:	48 ba 20 53 55 42 4d 	movabs $0x5254494d42555320,%rdx
  402a05:	49 54 52 
  402a08:	48 89 45 20          	mov    %rax,0x20(%rbp)
  402a0c:	48 89 55 28          	mov    %rdx,0x28(%rbp)
  402a10:	48 b8 5f 4d 41 58 42 	movabs $0x46554258414d5f,%rax
  402a17:	55 46 00 
  402a1a:	48 89 45 30          	mov    %rax,0x30(%rbp)
  402a1e:	89 df                	mov    %ebx,%edi
  402a20:	e8 fb e2 ff ff       	callq  400d20 <close@plt>
  402a25:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402a2a:	e9 56 fe ff ff       	jmpq   402885 <submitr+0x2e7>
  402a2f:	48 b8 45 72 72 6f 72 	movabs $0x52203a726f727245,%rax
  402a36:	3a 20 52 
  402a39:	48 ba 65 73 75 6c 74 	movabs $0x747320746c757365,%rdx
  402a40:	20 73 74 
  402a43:	48 89 45 00          	mov    %rax,0x0(%rbp)
  402a47:	48 89 55 08          	mov    %rdx,0x8(%rbp)
  402a4b:	48 b8 72 69 6e 67 20 	movabs $0x6e6f6320676e6972,%rax
  402a52:	63 6f 6e 
  402a55:	48 ba 74 61 69 6e 73 	movabs $0x6e6120736e696174,%rdx
  402a5c:	20 61 6e 
  402a5f:	48 89 45 10          	mov    %rax,0x10(%rbp)
  402a63:	48 89 55 18          	mov    %rdx,0x18(%rbp)
  402a67:	48 b8 20 69 6c 6c 65 	movabs $0x6c6167656c6c6920,%rax
  402a6e:	67 61 6c 
  402a71:	48 ba 20 6f 72 20 75 	movabs $0x72706e7520726f20,%rdx
  402a78:	6e 70 72 
  402a7b:	48 89 45 20          	mov    %rax,0x20(%rbp)
  402a7f:	48 89 55 28          	mov    %rdx,0x28(%rbp)
  402a83:	48 b8 69 6e 74 61 62 	movabs $0x20656c6261746e69,%rax
  402a8a:	6c 65 20 
  402a8d:	48 ba 63 68 61 72 61 	movabs $0x6574636172616863,%rdx
  402a94:	63 74 65 
  402a97:	48 89 45 30          	mov    %rax,0x30(%rbp)
  402a9b:	48 89 55 38          	mov    %rdx,0x38(%rbp)
  402a9f:	66 c7 45 40 72 2e    	movw   $0x2e72,0x40(%rbp)
  402aa5:	c6 45 42 00          	movb   $0x0,0x42(%rbp)
  402aa9:	89 df                	mov    %ebx,%edi
  402aab:	e8 70 e2 ff ff       	callq  400d20 <close@plt>
  402ab0:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402ab5:	e9 cb fd ff ff       	jmpq   402885 <submitr+0x2e7>
  402aba:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
  402ac1:	3a 20 43 
  402ac4:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
  402acb:	20 75 6e 
  402ace:	48 89 45 00          	mov    %rax,0x0(%rbp)
  402ad2:	48 89 55 08          	mov    %rdx,0x8(%rbp)
  402ad6:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
  402add:	74 6f 20 
  402ae0:	48 ba 77 72 69 74 65 	movabs $0x6f74206574697277,%rdx
  402ae7:	20 74 6f 
  402aea:	48 89 45 10          	mov    %rax,0x10(%rbp)
  402aee:	48 89 55 18          	mov    %rdx,0x18(%rbp)
  402af2:	48 b8 20 74 68 65 20 	movabs $0x7365722065687420,%rax
  402af9:	72 65 73 
  402afc:	48 ba 75 6c 74 20 73 	movabs $0x7672657320746c75,%rdx
  402b03:	65 72 76 
  402b06:	48 89 45 20          	mov    %rax,0x20(%rbp)
  402b0a:	48 89 55 28          	mov    %rdx,0x28(%rbp)
  402b0e:	66 c7 45 30 65 72    	movw   $0x7265,0x30(%rbp)
  402b14:	c6 45 32 00          	movb   $0x0,0x32(%rbp)
  402b18:	89 df                	mov    %ebx,%edi
  402b1a:	e8 01 e2 ff ff       	callq  400d20 <close@plt>
  402b1f:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402b24:	e9 5c fd ff ff       	jmpq   402885 <submitr+0x2e7>
  402b29:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
  402b30:	3a 20 43 
  402b33:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
  402b3a:	20 75 6e 
  402b3d:	48 89 45 00          	mov    %rax,0x0(%rbp)
  402b41:	48 89 55 08          	mov    %rdx,0x8(%rbp)
  402b45:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
  402b4c:	74 6f 20 
  402b4f:	48 ba 72 65 61 64 20 	movabs $0x7269662064616572,%rdx
  402b56:	66 69 72 
  402b59:	48 89 45 10          	mov    %rax,0x10(%rbp)
  402b5d:	48 89 55 18          	mov    %rdx,0x18(%rbp)
  402b61:	48 b8 73 74 20 68 65 	movabs $0x6564616568207473,%rax
  402b68:	61 64 65 
  402b6b:	48 ba 72 20 66 72 6f 	movabs $0x72206d6f72662072,%rdx
  402b72:	6d 20 72 
  402b75:	48 89 45 20          	mov    %rax,0x20(%rbp)
  402b79:	48 89 55 28          	mov    %rdx,0x28(%rbp)
  402b7d:	48 b8 65 73 75 6c 74 	movabs $0x657320746c757365,%rax
  402b84:	20 73 65 
  402b87:	48 89 45 30          	mov    %rax,0x30(%rbp)
  402b8b:	c7 45 38 72 76 65 72 	movl   $0x72657672,0x38(%rbp)
  402b92:	c6 45 3c 00          	movb   $0x0,0x3c(%rbp)
  402b96:	89 df                	mov    %ebx,%edi
  402b98:	e8 83 e1 ff ff       	callq  400d20 <close@plt>
  402b9d:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402ba2:	e9 de fc ff ff       	jmpq   402885 <submitr+0x2e7>
  402ba7:	48 8d b4 24 50 20 00 	lea    0x2050(%rsp),%rsi
  402bae:	00 
  402baf:	48 8d 7c 24 40       	lea    0x40(%rsp),%rdi
  402bb4:	ba 00 20 00 00       	mov    $0x2000,%edx
  402bb9:	e8 29 f8 ff ff       	callq  4023e7 <rio_readlineb>
  402bbe:	48 85 c0             	test   %rax,%rax
  402bc1:	0f 8e 96 00 00 00    	jle    402c5d <submitr+0x6bf>
  402bc7:	44 8b 44 24 2c       	mov    0x2c(%rsp),%r8d
  402bcc:	41 81 f8 c8 00 00 00 	cmp    $0xc8,%r8d
  402bd3:	0f 85 08 01 00 00    	jne    402ce1 <submitr+0x743>
  402bd9:	48 8d b4 24 50 20 00 	lea    0x2050(%rsp),%rsi
  402be0:	00 
  402be1:	48 89 ef             	mov    %rbp,%rdi
  402be4:	e8 c7 e0 ff ff       	callq  400cb0 <strcpy@plt>
  402be9:	89 df                	mov    %ebx,%edi
  402beb:	e8 30 e1 ff ff       	callq  400d20 <close@plt>
  402bf0:	b9 04 00 00 00       	mov    $0x4,%ecx
  402bf5:	48 8d 3d 74 0c 00 00 	lea    0xc74(%rip),%rdi        # 403870 <trans_char+0xc0>
  402bfc:	48 89 ee             	mov    %rbp,%rsi
  402bff:	f3 a6                	repz cmpsb %es:(%rdi),%ds:(%rsi)
  402c01:	0f 97 c0             	seta   %al
  402c04:	1c 00                	sbb    $0x0,%al
  402c06:	0f be c0             	movsbl %al,%eax
  402c09:	85 c0                	test   %eax,%eax
  402c0b:	0f 84 74 fc ff ff    	je     402885 <submitr+0x2e7>
  402c11:	b9 05 00 00 00       	mov    $0x5,%ecx
  402c16:	48 8d 3d 57 0c 00 00 	lea    0xc57(%rip),%rdi        # 403874 <trans_char+0xc4>
  402c1d:	48 89 ee             	mov    %rbp,%rsi
  402c20:	f3 a6                	repz cmpsb %es:(%rdi),%ds:(%rsi)
  402c22:	0f 97 c0             	seta   %al
  402c25:	1c 00                	sbb    $0x0,%al
  402c27:	0f be c0             	movsbl %al,%eax
  402c2a:	85 c0                	test   %eax,%eax
  402c2c:	0f 84 53 fc ff ff    	je     402885 <submitr+0x2e7>
  402c32:	b9 03 00 00 00       	mov    $0x3,%ecx
  402c37:	48 8d 3d 3b 0c 00 00 	lea    0xc3b(%rip),%rdi        # 403879 <trans_char+0xc9>
  402c3e:	48 89 ee             	mov    %rbp,%rsi
  402c41:	f3 a6                	repz cmpsb %es:(%rdi),%ds:(%rsi)
  402c43:	0f 97 c0             	seta   %al
  402c46:	1c 00                	sbb    $0x0,%al
  402c48:	0f be c0             	movsbl %al,%eax
  402c4b:	85 c0                	test   %eax,%eax
  402c4d:	0f 84 32 fc ff ff    	je     402885 <submitr+0x2e7>
  402c53:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402c58:	e9 28 fc ff ff       	jmpq   402885 <submitr+0x2e7>
  402c5d:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
  402c64:	3a 20 43 
  402c67:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
  402c6e:	20 75 6e 
  402c71:	48 89 45 00          	mov    %rax,0x0(%rbp)
  402c75:	48 89 55 08          	mov    %rdx,0x8(%rbp)
  402c79:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
  402c80:	74 6f 20 
  402c83:	48 ba 72 65 61 64 20 	movabs $0x6174732064616572,%rdx
  402c8a:	73 74 61 
  402c8d:	48 89 45 10          	mov    %rax,0x10(%rbp)
  402c91:	48 89 55 18          	mov    %rdx,0x18(%rbp)
  402c95:	48 b8 74 75 73 20 6d 	movabs $0x7373656d20737574,%rax
  402c9c:	65 73 73 
  402c9f:	48 ba 61 67 65 20 66 	movabs $0x6d6f726620656761,%rdx
  402ca6:	72 6f 6d 
  402ca9:	48 89 45 20          	mov    %rax,0x20(%rbp)
  402cad:	48 89 55 28          	mov    %rdx,0x28(%rbp)
  402cb1:	48 b8 20 72 65 73 75 	movabs $0x20746c7573657220,%rax
  402cb8:	6c 74 20 
  402cbb:	48 89 45 30          	mov    %rax,0x30(%rbp)
  402cbf:	c7 45 38 73 65 72 76 	movl   $0x76726573,0x38(%rbp)
  402cc6:	66 c7 45 3c 65 72    	movw   $0x7265,0x3c(%rbp)
  402ccc:	c6 45 3e 00          	movb   $0x0,0x3e(%rbp)
  402cd0:	89 df                	mov    %ebx,%edi
  402cd2:	e8 49 e0 ff ff       	callq  400d20 <close@plt>
  402cd7:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402cdc:	e9 a4 fb ff ff       	jmpq   402885 <submitr+0x2e7>
  402ce1:	4c 8d 8c 24 50 80 00 	lea    0x8050(%rsp),%r9
  402ce8:	00 
  402ce9:	48 8d 0d 38 0b 00 00 	lea    0xb38(%rip),%rcx        # 403828 <trans_char+0x78>
  402cf0:	48 c7 c2 ff ff ff ff 	mov    $0xffffffffffffffff,%rdx
  402cf7:	be 01 00 00 00       	mov    $0x1,%esi
  402cfc:	48 89 ef             	mov    %rbp,%rdi
  402cff:	b8 00 00 00 00       	mov    $0x0,%eax
  402d04:	e8 57 e1 ff ff       	callq  400e60 <__sprintf_chk@plt>
  402d09:	89 df                	mov    %ebx,%edi
  402d0b:	e8 10 e0 ff ff       	callq  400d20 <close@plt>
  402d10:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402d15:	e9 6b fb ff ff       	jmpq   402885 <submitr+0x2e7>
  402d1a:	e8 c1 df ff ff       	callq  400ce0 <__stack_chk_fail@plt>

0000000000402d1f <init_timeout>:
  402d1f:	85 ff                	test   %edi,%edi
  402d21:	74 28                	je     402d4b <init_timeout+0x2c>
  402d23:	53                   	push   %rbx
  402d24:	89 fb                	mov    %edi,%ebx
  402d26:	85 ff                	test   %edi,%edi
  402d28:	78 1a                	js     402d44 <init_timeout+0x25>
  402d2a:	48 8d 35 9d f5 ff ff 	lea    -0xa63(%rip),%rsi        # 4022ce <sigalrm_handler>
  402d31:	bf 0e 00 00 00       	mov    $0xe,%edi
  402d36:	e8 05 e0 ff ff       	callq  400d40 <signal@plt>
  402d3b:	89 df                	mov    %ebx,%edi
  402d3d:	e8 ce df ff ff       	callq  400d10 <alarm@plt>
  402d42:	5b                   	pop    %rbx
  402d43:	c3                   	retq   
  402d44:	bb 00 00 00 00       	mov    $0x0,%ebx
  402d49:	eb df                	jmp    402d2a <init_timeout+0xb>
  402d4b:	f3 c3                	repz retq 

0000000000402d4d <init_driver>:
  402d4d:	41 54                	push   %r12
  402d4f:	55                   	push   %rbp
  402d50:	53                   	push   %rbx
  402d51:	48 83 ec 20          	sub    $0x20,%rsp
  402d55:	49 89 fc             	mov    %rdi,%r12
  402d58:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  402d5f:	00 00 
  402d61:	48 89 44 24 18       	mov    %rax,0x18(%rsp)
  402d66:	31 c0                	xor    %eax,%eax
  402d68:	be 01 00 00 00       	mov    $0x1,%esi
  402d6d:	bf 0d 00 00 00       	mov    $0xd,%edi
  402d72:	e8 c9 df ff ff       	callq  400d40 <signal@plt>
  402d77:	be 01 00 00 00       	mov    $0x1,%esi
  402d7c:	bf 1d 00 00 00       	mov    $0x1d,%edi
  402d81:	e8 ba df ff ff       	callq  400d40 <signal@plt>
  402d86:	be 01 00 00 00       	mov    $0x1,%esi
  402d8b:	bf 1d 00 00 00       	mov    $0x1d,%edi
  402d90:	e8 ab df ff ff       	callq  400d40 <signal@plt>
  402d95:	ba 00 00 00 00       	mov    $0x0,%edx
  402d9a:	be 01 00 00 00       	mov    $0x1,%esi
  402d9f:	bf 02 00 00 00       	mov    $0x2,%edi
  402da4:	e8 c7 e0 ff ff       	callq  400e70 <socket@plt>
  402da9:	85 c0                	test   %eax,%eax
  402dab:	0f 88 a3 00 00 00    	js     402e54 <init_driver+0x107>
  402db1:	89 c3                	mov    %eax,%ebx
  402db3:	48 8d 3d c2 0a 00 00 	lea    0xac2(%rip),%rdi        # 40387c <trans_char+0xcc>
  402dba:	e8 91 df ff ff       	callq  400d50 <gethostbyname@plt>
  402dbf:	48 85 c0             	test   %rax,%rax
  402dc2:	0f 84 df 00 00 00    	je     402ea7 <init_driver+0x15a>
  402dc8:	48 89 e5             	mov    %rsp,%rbp
  402dcb:	48 c7 44 24 02 00 00 	movq   $0x0,0x2(%rsp)
  402dd2:	00 00 
  402dd4:	c7 45 0a 00 00 00 00 	movl   $0x0,0xa(%rbp)
  402ddb:	66 c7 45 0e 00 00    	movw   $0x0,0xe(%rbp)
  402de1:	66 c7 04 24 02 00    	movw   $0x2,(%rsp)
  402de7:	48 63 50 14          	movslq 0x14(%rax),%rdx
  402deb:	48 8b 40 18          	mov    0x18(%rax),%rax
  402def:	48 8b 30             	mov    (%rax),%rsi
  402df2:	48 8d 7d 04          	lea    0x4(%rbp),%rdi
  402df6:	b9 0c 00 00 00       	mov    $0xc,%ecx
  402dfb:	e8 60 df ff ff       	callq  400d60 <__memmove_chk@plt>
  402e00:	66 c7 44 24 02 3c 9a 	movw   $0x9a3c,0x2(%rsp)
  402e07:	ba 10 00 00 00       	mov    $0x10,%edx
  402e0c:	48 89 ee             	mov    %rbp,%rsi
  402e0f:	89 df                	mov    %ebx,%edi
  402e11:	e8 2a e0 ff ff       	callq  400e40 <connect@plt>
  402e16:	85 c0                	test   %eax,%eax
  402e18:	0f 88 fb 00 00 00    	js     402f19 <init_driver+0x1cc>
  402e1e:	89 df                	mov    %ebx,%edi
  402e20:	e8 fb de ff ff       	callq  400d20 <close@plt>
  402e25:	66 41 c7 04 24 4f 4b 	movw   $0x4b4f,(%r12)
  402e2c:	41 c6 44 24 02 00    	movb   $0x0,0x2(%r12)
  402e32:	b8 00 00 00 00       	mov    $0x0,%eax
  402e37:	48 8b 4c 24 18       	mov    0x18(%rsp),%rcx
  402e3c:	64 48 33 0c 25 28 00 	xor    %fs:0x28,%rcx
  402e43:	00 00 
  402e45:	0f 85 28 01 00 00    	jne    402f73 <init_driver+0x226>
  402e4b:	48 83 c4 20          	add    $0x20,%rsp
  402e4f:	5b                   	pop    %rbx
  402e50:	5d                   	pop    %rbp
  402e51:	41 5c                	pop    %r12
  402e53:	c3                   	retq   
  402e54:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
  402e5b:	3a 20 43 
  402e5e:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
  402e65:	20 75 6e 
  402e68:	49 89 04 24          	mov    %rax,(%r12)
  402e6c:	49 89 54 24 08       	mov    %rdx,0x8(%r12)
  402e71:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
  402e78:	74 6f 20 
  402e7b:	48 ba 63 72 65 61 74 	movabs $0x7320657461657263,%rdx
  402e82:	65 20 73 
  402e85:	49 89 44 24 10       	mov    %rax,0x10(%r12)
  402e8a:	49 89 54 24 18       	mov    %rdx,0x18(%r12)
  402e8f:	41 c7 44 24 20 6f 63 	movl   $0x656b636f,0x20(%r12)
  402e96:	6b 65 
  402e98:	66 41 c7 44 24 24 74 	movw   $0x74,0x24(%r12)
  402e9f:	00 
  402ea0:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402ea5:	eb 90                	jmp    402e37 <init_driver+0xea>
  402ea7:	48 b8 45 72 72 6f 72 	movabs $0x44203a726f727245,%rax
  402eae:	3a 20 44 
  402eb1:	48 ba 4e 53 20 69 73 	movabs $0x6e7520736920534e,%rdx
  402eb8:	20 75 6e 
  402ebb:	49 89 04 24          	mov    %rax,(%r12)
  402ebf:	49 89 54 24 08       	mov    %rdx,0x8(%r12)
  402ec4:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
  402ecb:	74 6f 20 
  402ece:	48 ba 72 65 73 6f 6c 	movabs $0x2065766c6f736572,%rdx
  402ed5:	76 65 20 
  402ed8:	49 89 44 24 10       	mov    %rax,0x10(%r12)
  402edd:	49 89 54 24 18       	mov    %rdx,0x18(%r12)
  402ee2:	48 b8 73 65 72 76 65 	movabs $0x6120726576726573,%rax
  402ee9:	72 20 61 
  402eec:	49 89 44 24 20       	mov    %rax,0x20(%r12)
  402ef1:	41 c7 44 24 28 64 64 	movl   $0x65726464,0x28(%r12)
  402ef8:	72 65 
  402efa:	66 41 c7 44 24 2c 73 	movw   $0x7373,0x2c(%r12)
  402f01:	73 
  402f02:	41 c6 44 24 2e 00    	movb   $0x0,0x2e(%r12)
  402f08:	89 df                	mov    %ebx,%edi
  402f0a:	e8 11 de ff ff       	callq  400d20 <close@plt>
  402f0f:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402f14:	e9 1e ff ff ff       	jmpq   402e37 <init_driver+0xea>
  402f19:	48 b8 45 72 72 6f 72 	movabs $0x55203a726f727245,%rax
  402f20:	3a 20 55 
  402f23:	48 ba 6e 61 62 6c 65 	movabs $0x6f7420656c62616e,%rdx
  402f2a:	20 74 6f 
  402f2d:	49 89 04 24          	mov    %rax,(%r12)
  402f31:	49 89 54 24 08       	mov    %rdx,0x8(%r12)
  402f36:	48 b8 20 63 6f 6e 6e 	movabs $0x7463656e6e6f6320,%rax
  402f3d:	65 63 74 
  402f40:	48 ba 20 74 6f 20 73 	movabs $0x76726573206f7420,%rdx
  402f47:	65 72 76 
  402f4a:	49 89 44 24 10       	mov    %rax,0x10(%r12)
  402f4f:	49 89 54 24 18       	mov    %rdx,0x18(%r12)
  402f54:	66 41 c7 44 24 20 65 	movw   $0x7265,0x20(%r12)
  402f5b:	72 
  402f5c:	41 c6 44 24 22 00    	movb   $0x0,0x22(%r12)
  402f62:	89 df                	mov    %ebx,%edi
  402f64:	e8 b7 dd ff ff       	callq  400d20 <close@plt>
  402f69:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  402f6e:	e9 c4 fe ff ff       	jmpq   402e37 <init_driver+0xea>
  402f73:	e8 68 dd ff ff       	callq  400ce0 <__stack_chk_fail@plt>

0000000000402f78 <driver_post>:
  402f78:	53                   	push   %rbx
  402f79:	4c 89 cb             	mov    %r9,%rbx
  402f7c:	45 85 c0             	test   %r8d,%r8d
  402f7f:	75 18                	jne    402f99 <driver_post+0x21>
  402f81:	48 85 ff             	test   %rdi,%rdi
  402f84:	74 05                	je     402f8b <driver_post+0x13>
  402f86:	80 3f 00             	cmpb   $0x0,(%rdi)
  402f89:	75 37                	jne    402fc2 <driver_post+0x4a>
  402f8b:	66 c7 03 4f 4b       	movw   $0x4b4f,(%rbx)
  402f90:	c6 43 02 00          	movb   $0x0,0x2(%rbx)
  402f94:	44 89 c0             	mov    %r8d,%eax
  402f97:	5b                   	pop    %rbx
  402f98:	c3                   	retq   
  402f99:	48 89 ca             	mov    %rcx,%rdx
  402f9c:	48 8d 35 f1 08 00 00 	lea    0x8f1(%rip),%rsi        # 403894 <trans_char+0xe4>
  402fa3:	bf 01 00 00 00       	mov    $0x1,%edi
  402fa8:	b8 00 00 00 00       	mov    $0x0,%eax
  402fad:	e8 2e de ff ff       	callq  400de0 <__printf_chk@plt>
  402fb2:	66 c7 03 4f 4b       	movw   $0x4b4f,(%rbx)
  402fb7:	c6 43 02 00          	movb   $0x0,0x2(%rbx)
  402fbb:	b8 00 00 00 00       	mov    $0x0,%eax
  402fc0:	eb d5                	jmp    402f97 <driver_post+0x1f>
  402fc2:	48 83 ec 08          	sub    $0x8,%rsp
  402fc6:	41 51                	push   %r9
  402fc8:	49 89 c9             	mov    %rcx,%r9
  402fcb:	49 89 d0             	mov    %rdx,%r8
  402fce:	48 89 f9             	mov    %rdi,%rcx
  402fd1:	48 89 f2             	mov    %rsi,%rdx
  402fd4:	be 9a 3c 00 00       	mov    $0x3c9a,%esi
  402fd9:	48 8d 3d 9c 08 00 00 	lea    0x89c(%rip),%rdi        # 40387c <trans_char+0xcc>
  402fe0:	e8 b9 f5 ff ff       	callq  40259e <submitr>
  402fe5:	48 83 c4 10          	add    $0x10,%rsp
  402fe9:	eb ac                	jmp    402f97 <driver_post+0x1f>

0000000000402feb <check>:
  402feb:	89 f8                	mov    %edi,%eax
  402fed:	c1 e8 1c             	shr    $0x1c,%eax
  402ff0:	85 c0                	test   %eax,%eax
  402ff2:	74 1d                	je     403011 <check+0x26>
  402ff4:	b9 00 00 00 00       	mov    $0x0,%ecx
  402ff9:	83 f9 1f             	cmp    $0x1f,%ecx
  402ffc:	7f 0d                	jg     40300b <check+0x20>
  402ffe:	89 f8                	mov    %edi,%eax
  403000:	d3 e8                	shr    %cl,%eax
  403002:	3c 0a                	cmp    $0xa,%al
  403004:	74 11                	je     403017 <check+0x2c>
  403006:	83 c1 08             	add    $0x8,%ecx
  403009:	eb ee                	jmp    402ff9 <check+0xe>
  40300b:	b8 01 00 00 00       	mov    $0x1,%eax
  403010:	c3                   	retq   
  403011:	b8 00 00 00 00       	mov    $0x0,%eax
  403016:	c3                   	retq   
  403017:	b8 00 00 00 00       	mov    $0x0,%eax
  40301c:	c3                   	retq   

000000000040301d <gencookie>:
  40301d:	53                   	push   %rbx
  40301e:	83 c7 01             	add    $0x1,%edi
  403021:	e8 6a dc ff ff       	callq  400c90 <srandom@plt>
  403026:	e8 75 dd ff ff       	callq  400da0 <random@plt>
  40302b:	89 c3                	mov    %eax,%ebx
  40302d:	89 c7                	mov    %eax,%edi
  40302f:	e8 b7 ff ff ff       	callq  402feb <check>
  403034:	85 c0                	test   %eax,%eax
  403036:	74 ee                	je     403026 <gencookie+0x9>
  403038:	89 d8                	mov    %ebx,%eax
  40303a:	5b                   	pop    %rbx
  40303b:	c3                   	retq   
  40303c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000403040 <__libc_csu_init>:
  403040:	41 57                	push   %r15
  403042:	41 56                	push   %r14
  403044:	49 89 d7             	mov    %rdx,%r15
  403047:	41 55                	push   %r13
  403049:	41 54                	push   %r12
  40304b:	4c 8d 25 be 1d 20 00 	lea    0x201dbe(%rip),%r12        # 604e10 <__frame_dummy_init_array_entry>
  403052:	55                   	push   %rbp
  403053:	48 8d 2d be 1d 20 00 	lea    0x201dbe(%rip),%rbp        # 604e18 <__init_array_end>
  40305a:	53                   	push   %rbx
  40305b:	41 89 fd             	mov    %edi,%r13d
  40305e:	49 89 f6             	mov    %rsi,%r14
  403061:	4c 29 e5             	sub    %r12,%rbp
  403064:	48 83 ec 08          	sub    $0x8,%rsp
  403068:	48 c1 fd 03          	sar    $0x3,%rbp
  40306c:	e8 d7 db ff ff       	callq  400c48 <_init>
  403071:	48 85 ed             	test   %rbp,%rbp
  403074:	74 20                	je     403096 <__libc_csu_init+0x56>
  403076:	31 db                	xor    %ebx,%ebx
  403078:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
  40307f:	00 
  403080:	4c 89 fa             	mov    %r15,%rdx
  403083:	4c 89 f6             	mov    %r14,%rsi
  403086:	44 89 ef             	mov    %r13d,%edi
  403089:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
  40308d:	48 83 c3 01          	add    $0x1,%rbx
  403091:	48 39 dd             	cmp    %rbx,%rbp
  403094:	75 ea                	jne    403080 <__libc_csu_init+0x40>
  403096:	48 83 c4 08          	add    $0x8,%rsp
  40309a:	5b                   	pop    %rbx
  40309b:	5d                   	pop    %rbp
  40309c:	41 5c                	pop    %r12
  40309e:	41 5d                	pop    %r13
  4030a0:	41 5e                	pop    %r14
  4030a2:	41 5f                	pop    %r15
  4030a4:	c3                   	retq   
  4030a5:	90                   	nop
  4030a6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  4030ad:	00 00 00 

00000000004030b0 <__libc_csu_fini>:
  4030b0:	f3 c3                	repz retq 

Disassembly of section .fini:

00000000004030b4 <_fini>:
  4030b4:	48 83 ec 08          	sub    $0x8,%rsp
  4030b8:	48 83 c4 08          	add    $0x8,%rsp
  4030bc:	c3                   	retq   
