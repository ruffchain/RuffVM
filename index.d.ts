export class Script {
    setSandbox(sandbox: any): this;
    setOption(option: any): this;
    setUserCode(str: string): this;
    runAsync(): Promise<void>;
}

export function createScript(type: string): Script;
export function resolveHelper(resolve: any, timeoutMs: number| undefined): (value:any) => void;
